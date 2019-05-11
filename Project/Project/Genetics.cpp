#include "Genetics.h"

#include <random>
#include <iostream>
#include <map>
#include <set>
#include <iterator>     

Genetics::Genetics(Data* data, Node initial) : Genetics(data, initial, 50, 1000, 30, 15)
{
}

Genetics::Genetics(Data* data, Node initial, int populationSize, int maxNoGenerations, int mutationProbability, int parentEliteNo)
{
	this->data = data;
	this->initial = initial;
	this->best = initial;
	this->populationSize = populationSize;
	this->maxNoGenerations = maxNoGenerations;
	this->mutationProbability = mutationProbability;
	this->parentEliteNo = parentEliteNo;
}


void Genetics::printBest()
{
	this->best.print();
}

//change to choose from the best
Node Genetics::randomSelection(std::set<Node>* population)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int n = mt() % population->size();
	int index = 0;
	auto it = population->begin();
	std::advance(it, n);
	return (*it);
}


void Genetics::mutate(Node* elem)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int n = mt() % 100 + 1;  //random number between 1 and 100

	if (n > this->mutationProbability)
		return;

	int nrOfMutations = mt() % elem->getAnswersSize() + 1;
	int index, randomPeriod, randomRoom;

	for (int i = 0; i < nrOfMutations; i++) {
		 index = mt() % elem->getAnswersSize();
		 randomPeriod = mt() % this->data->getPeriodsCnt() ;
		 randomRoom = mt() % this->data->getRoomsCnt();
		 elem->setAnswer(index, randomPeriod, randomRoom);
	}
}

/*
	pair<period, room>
std::vector<std::pair<int, int>> answers; */
Node Genetics::reproduce(Node* elem1, Node* elem2)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int c = mt() % elem1->getAnswersSize() ;

	std::vector<std::pair<int, int>> elemAnswers = elem1->getAnswers();
	std::vector<std::pair<int, int>> answers1(elemAnswers.begin(), elemAnswers.begin()+ c);
	elemAnswers = elem2->getAnswers();
	std::vector<std::pair<int, int>> answers2(elemAnswers.begin() + c, elemAnswers.begin() + elem2->getAnswersSize());
	answers1.insert(answers1.end(), answers2.begin(), answers2.end());

	Node child = Node();
	child.setAnswers(answers1);
	return child;
}

//permite elitismo, manter os parentEliteNo progenitores na populaçao
void Genetics::insertPopulationBestElements(std::set<Node>* prevPopulation, std::set<Node>* newPopulation)
{
	int ad = this->parentEliteNo;
	if (prevPopulation->size() < this->parentEliteNo)
		ad = prevPopulation->size();
	auto bg = prevPopulation->begin();
	std::advance(bg, ad);
	newPopulation->insert(prevPopulation->begin(), bg );
}

// fazer refactoring nesta porra
void Genetics::evaluateSolution(Node* solution)
{
	int penalty = 0, noFaults = 0, roomIndex, periodIndex;
	std::string roomConstraint, periodConstraint;

	std::vector<std::pair<int, int>> schedule = solution->getAnswers();
	std::vector<Exam> exams = this->data->getExams(), examsSorted = exams;
	std::vector<Period> periods = this->data->getPeriods();
	std::vector<Room> rooms = this->data->getRooms();

	std::map<std::pair<int, int>, int> examSlot; //key : periodIndex, roomIndex, value: roomOcSpace
	//key : periodIndex
	//value : pair : set -> key = exam duration ; vector : exam index
	std::map<int, std::pair<std::set<int>, std::vector<int>>> periodInfo;
	std::map<std::string, std::vector<int>> examDays;

	InstitutionalWeightings instWeights = this->data->getInstWeights();
	int flNrExams = instWeights.getFrontLoad().getNrExams();
	int flNrPeriods = instWeights.getFrontLoad().getNrExams();
	int flPenalty = instWeights.getFrontLoad().getPenalty();

	std::sort(examsSorted.begin(), examsSorted.end()); //sorted by student count

	for (int i = 0; i < schedule.size(); i++) {

		Exam exam = exams.at(i);

		periodIndex = schedule.at(i).first;
		Period period = periods.at(periodIndex);

		auto itp = periodInfo.find(periodIndex);
		if (itp != periodInfo.end()) {
			itp->second.first.insert(exam.getDuration());
			itp->second.second.push_back(i);
		}
		else {
			std::set<int> durations;
			durations.insert(exam.getDuration());
			std::vector<int> pExams;
			pExams.push_back(i);
			periodInfo.insert(std::pair<int, std::pair<std::set<int>, std::vector<int>>>(periodIndex, std::pair<std::set<int>, std::vector<int>>(durations, pExams)));
		}
		
		roomIndex = schedule.at(i).second;
		Room room = rooms.at(roomIndex);
	
		noFaults += applyGeneralHardConstraints(i, &examSlot, &schedule, &exam, &period, &room);
		noFaults += applyPeriodHardConstraints(i, &periods, &exams, &schedule, &period, &exam, periodIndex);

		penalty += room.getPenalty(); //penalty of using the room
		penalty += period.getPenalty(); //penalty of using the period

		//Larger Exams towards the beginning of the examination session constraint
		auto it = find(examsSorted.begin(), examsSorted.end(), exam);
		int pos = distance(examsSorted.begin(), it);
		
		if (pos >= flNrExams && periodIndex >= flNrPeriods) 
			penalty += flPenalty;
	}

	std::map<std::string, std::vector<int>> periodDays = this->data->getPeriodDays();
	int periodPenalty = 0 , currentPeriodIndex, overlappingNo;

	//percorre todos os periodos e ve para cada 1
	//*a cena das mixed durations
	//*os periodos no mesmo dia
	//*para cada exame do periodo ve as colisoes com os outros exames do mesmo periodo
	//*para cada exame do periodo ve as colisoes com os exames dos outros periodos do mesmo dia (os periodos depois dele)

	for (auto it = periodInfo.begin(); it != periodInfo.end(); it++){

		currentPeriodIndex = it->first;
		
		//Mixed Durations constraint
		penalty += (it->second.first.size() - 1) * instWeights.getNonMixedDurations();
		
		std::vector<int> periodExams = it->second.second; //todos os exames do periodo de index it->first

		std::string sDate = periods.at(currentPeriodIndex).getDate().getDate(); //dia do periodo 
		std::vector<int> sameDayPeriods = periodDays[sDate]; //todos os periodos daquele dia

		int sameDayPeriodIndex;  //index do periodo daquele dia
		std::vector<int> sameDayExams;

		for (int i = 0; i < sameDayPeriods.size(); i++) {

			sameDayPeriodIndex = sameDayPeriods.at(i);

			if (sameDayPeriodIndex > currentPeriodIndex + 1) {  //sameday
				periodPenalty = instWeights.getTwoInDay();
			}
			else if (sameDayPeriodIndex == currentPeriodIndex + 1) {  //inrow
				periodPenalty = instWeights.getTwoInRow();
			}
			else
				continue;

			auto iter = periodInfo.find(sameDayPeriodIndex);
			if(iter == periodInfo.end())
				continue;

			sameDayExams = iter->second.second;
			int e1, e2;
			for (int k = 0; k < periodExams.size(); k++) {
				e1 = periodExams.at(k);
				for (int j = 0; j < sameDayExams.size(); j++) {
					e2 = sameDayExams.at(j);
					overlappingNo = this->data->getExamsOverlaps(e1,e2);
					penalty += overlappingNo * periodPenalty;
				}
			}
		}

		int e1, e2;
		for (int i = 0; i < periodExams.size() ; i++) {
			e1 = periodExams.at(i);

			//ve as colisoes de todos os exames de um periodo
			for (int j = i + 1; j < periodExams.size(); j++) {
				e2 = periodExams.at(j);
				if (this->data->getExamsOverlaps(e1,e2) != 0) {
					noFaults++;
				} 
			}
			
			//Period Spread constraint
			for (int j = currentPeriodIndex + 1; j < periods.size(); j++) {

				auto iter = periodInfo.find(j);
				if (iter == periodInfo.end())
					continue;
				for (int k = 0; k < iter->second.second.size(); k++) {
					e2 = iter->second.second.at(k);
					overlappingNo = this->data->getExamsOverlaps(e1,e2);
					penalty += overlappingNo;
				}
				if (j >= instWeights.getPeriodSpreed())
					break;
			}
		}	
	}

	solution->incNoFaults(noFaults);
	solution->incPenalty(penalty);
}

int Genetics::applyPeriodHardConstraints(int index,  std::vector<Period> *periods, std::vector<Exam>* exams, std::vector<std::pair<int, int>>* schedule, Period* period, Exam* exam, int periodIndex)
{
	std::string periodConstraint;
	std::multimap<int, std::pair<int, std::string>> periodConstraints = this->data->getPeriodConstraints();
	auto  ret = periodConstraints.equal_range(index);
	int exam2Index, noFaults = 0, period2Index;
	for (auto it = ret.first; it != ret.second; ++it) {
		periodConstraint = it->second.second;
		exam2Index = it->second.first;
		period2Index = schedule->at(exam2Index).first;

		if (periodConstraint.compare("AFTER") == 0) {
			Period period2 = periods->at(period2Index);
			if (period->getDate() == period2.getDate()) {
				if (period->getTime() <= period2.getTime())
					noFaults++;
			}
			else if (period->getDate() < period2.getDate()) {
				noFaults++;
			}
		}
		else if (periodConstraint.compare("EXCLUSION") == 0) {
			if (periodIndex == period2Index)
				noFaults++;
		}
		else if (periodConstraint.compare("EXAM_COINCIDENCE") == 0) {
			if (periodIndex != period2Index) {
				//Exam exam2 = exams->at(exam2Index);
				if(this->data->getExamsOverlaps(index, exam2Index) == 0)
					noFaults++;
			}
		}
	}

	return noFaults;
}


int Genetics::applyGeneralHardConstraints( int index, std::map<std::pair<int, int>, int>* examSlot, std::vector<std::pair<int, int>>* schedule, Exam* exam, Period* period, Room* room)
{
	int noFaults = 0;
	std::string roomConstraint = this->data->getExamRoomConstraint(index);

	auto it = examSlot->find(schedule->at(index));
	if (it != examSlot->end()) {
		it->second = it->second + exam->getStudentsCnt();

		if (roomConstraint.compare("ROOM_EXCLUSIVE") == 0) {  //TODO USE MACRO
			noFaults++;
		}
	}
	else {
		it = (examSlot->insert(std::pair<std::pair<int, int>, int>(schedule->at(index), exam->getStudentsCnt()))).first;
	}

	if (period->getDuration() < exam->getDuration())
		noFaults++;  //penalty of exceding the period's duration
	if (room->getCapacity() < it->second )
		noFaults++;  //penalty of exceding the room's capacity

	return noFaults;
}


//o best vai ser o que tiver menor penalty 
Node Genetics::solve(std::set<Node> population)
{
	int bestPenalty = -1, bestNoFaults, penalty, noFaults;
	int maxRoomPeriodPenalty = this->data->getMaxRoomPenalty() + this->data->getMaxPeriodPenalty();
	Node elem1, elem2, child;

	for (int generationsCount = 0; generationsCount < this->maxNoGenerations; generationsCount++) {

		std::set<Node> newPopulation;
		insertPopulationBestElements(&population, &newPopulation);

		//todo refactor -> mudar para outra função
		for (int currentPopulationSize = 0; currentPopulationSize < this->populationSize; currentPopulationSize++) {

			elem1 = this->randomSelection(&population);
			elem2 = this->randomSelection(&population);
		
			child = reproduce(&elem1, &elem2);
			mutate(&child);

			evaluateSolution(&child);

			penalty = child.getPenalty();
			noFaults = child.getNoFaults();
			if (bestPenalty == -1 || (penalty <= bestPenalty && noFaults <= bestNoFaults) || noFaults < bestNoFaults) {
				this->best = child;
				bestPenalty = penalty;
				bestNoFaults = noFaults;
			}

			//todo este or e estupido, arranjar outra condiçao de paragem
			if (noFaults == 0 && penalty == 0) /*
				|| (this->data->getExamsCnt() == this->data->getPeriodsCnt() && this->data->getExamsCnt() == this->data->getRoomsCnt()
					&& penalty == maxRoomPeriodPenalty)))*/ {
				
				return this->best;
			}
		
			newPopulation.insert(child);
		}
		population = newPopulation;
	}
	return this->best;
}


