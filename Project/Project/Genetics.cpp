#include "Genetics.h"

#include <random>
#include <iostream>
#include <map>
#include <set>

Genetics::Genetics(Data* data, Node initial) : Genetics(data, initial, 50, 1000, 20, 2)
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
Node Genetics::randomSelection(std::unordered_set<Node>* population)
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

//permite elitismo, manter os parentEliteNo progenitores na populaçao, se as suas penalties forem superiores ou iguais a melhor atual
void Genetics::insertPopulationBestElements(std::unordered_set<Node>* prevPopulation, std::unordered_set<Node>* newPopulation)
{

}

//mudar penalidades mais pesadas para serem do estilo constante*nrExames ou algo assim
// fazer refactoring nesta porra
void Genetics::evaluateSolution(Node* solution)
{
	int penalty = 0, noFaults = 0, roomIndex, periodIndex;
	std::string roomConstraint, periodConstraint;

	std::vector<std::pair<int, int>> schedule = solution->getAnswers();
	std::vector<Exam> exams = this->data->getExams(), examsSorted = exams;
	std::vector<Period> periods = this->data->getPeriods();
	std::vector<Room> rooms = this->data->getRooms();

	std::map<std::pair<int, int>, std::pair<int, int>> examSlot; //key : periodIndex, roomIndex, value: periodOcTime roomOcSpace
	std::map<int, std::set<int>> periodDurations; //key periodIndex  value: set with durations of the exams -> used for Mixed Durations penalty
	
	std::sort(examsSorted.begin(), examsSorted.end()); //sorted by student count

	for (int i = 0; i < schedule.size(); i++) {

		Exam exam = exams.at(i);

		periodIndex = schedule.at(i).first;
		Period period = periods.at(periodIndex);

		roomIndex = schedule.at(i).second;
		Room room = rooms.at(roomIndex);

		noFaults += applyGeneralHardConstraints(i, &examSlot, &schedule, &exam, &period, &room);
		noFaults += applyPeriodHardConstraints(i, &periods, &schedule, &period, periodIndex);

		penalty += room.getPenalty(); //penalty of using the room
		penalty += period.getPenalty(); //penalty of using the period

		auto it = find(examsSorted.begin(), examsSorted.end(), exam);
		int pos = distance(examsSorted.begin(), it);
		
		/*InstitutionalWeightings instWeights = this->data->getInstWeights();
		int flNrExams = instWeights.getFrontLoad().getNrExams();
		int flNrPeriods = instWeights.getFrontLoad().getNrExams();
		int flPenalty = instWeights.getFrontLoad().getPenalty();

		if (pos >= flNrExams && periodIndex >= flNrPeriods) //so funciona quando os periodos forem ordenados por "hora"
			penalty += flPenalty;*/


		auto itr = periodDurations.find(periodIndex);
		if (itr != periodDurations.end()) {
			itr->second.insert(exam.getDuration());
		}
		else {
			std::set<int> durations;
			durations.insert(exam.getDuration());
			periodDurations.insert(std::pair<int, std::set<int>>(periodIndex, durations));
		}
		
	}

	for (auto it = periodDurations.begin(); it != periodDurations.end(); it++){
		penalty += (it->second.size() - 1) * this->data->getInstWeights().getNonMixedDurations();
	}

	solution->incNoFaults(noFaults);
	solution->incPenalty(penalty);
}

int Genetics::applyPeriodHardConstraints(int index,  std::vector<Period> *periods, std::vector<std::pair<int, int>>* schedule, Period* period,  int periodIndex)
{
	std::string periodConstraint;
	std::multimap<int, std::pair<int, std::string>> periodConstraints = this->data->getPeriodConstraints();
	auto  ret = periodConstraints.equal_range(index);
	int exam2, noFaults = 0;
	for (auto it = ret.first; it != ret.second; ++it) {
		periodConstraint = it->second.second;
		exam2 = it->second.first;
	
		if (periodConstraint.compare("AFTER") == 0) {
			Period period2 = periods->at(schedule->at(exam2).first);
			if (period->getDate() == period2.getDate()) {
				if (period->getTime() <= period2.getTime())
					noFaults++;
			}
			else if (period->getDate() < period2.getDate()) {
				noFaults++;
			}
		}
		else if (periodConstraint.compare("EXCLUSION") == 0) {
			if (periodIndex == schedule->at(exam2).first)
				noFaults++;
		}
		else if (periodConstraint.compare("EXAM_COINCIDENCE") == 0) {
			if (periodIndex != schedule->at(exam2).first) //TODO CENA DAS SOBREPOSIÇOES DE ALUNOS
				noFaults++;
		}
	}

	return noFaults;
}


int Genetics::applyGeneralHardConstraints( int index, std::map<std::pair<int, int>, std::pair<int, int>>* examSlot, std::vector<std::pair<int, int>>* schedule, Exam* exam, Period* period, Room* room)
{
	int noFaults = 0;
	std::string roomConstraint = this->data->getExamRoomConstraint(index);

	auto it = examSlot->find(schedule->at(index));
	if (it != examSlot->end()) {
		it->second.first = it->second.first + exam->getDuration();
		it->second.second = it->second.second + exam->getStudentsCnt();

		if (roomConstraint.compare("ROOM_EXCLUSIVE") == 0) {  //TODO USE MACRO
			noFaults++;
		}
	}
	else {
		it = (examSlot->insert(std::pair<std::pair<int, int>, std::pair<int, int>>(schedule->at(index), std::pair<int, int>(exam->getDuration(), exam->getStudentsCnt())))).first;
	}

	if (period->getDuration() - it->second.first < 0)
		noFaults++;  //penalty of exceding the period's duration
	if (room->getCapacity() - it->second.second < 0)
		noFaults++;  //penalty of exceding the room's capacity

	return noFaults;
}


//o best vai ser o que tiver menor penalty 
//mudar para usar set normal com funçao de ordenaçao pela penalty do node (acresecentar field ao Node?)
Node Genetics::solve(std::unordered_set<Node> population)
{
	int bestPenalty = -1, bestNoFaults, penalty, noFaults;
	int maxRoomPeriodPenalty = this->data->getMaxRoomPenalty() + this->data->getMaxPeriodPenalty();
	Node elem1, elem2, child;

	for (int generationsCount = 0; generationsCount < this->maxNoGenerations; generationsCount++) {

		std::unordered_set<Node> newPopulation;
		//insertPopulationBestElements(&population, &newPopulation);

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

			if (noFaults == 0 && (penalty == 0
				|| (this->data->getExamsCnt() == this->data->getPeriodsCnt() && this->data->getExamsCnt() == this->data->getRoomsCnt()
					&& penalty == maxRoomPeriodPenalty))) {
				
				this->best.print();
				std::cout << this->best.getNoFaults() << std::endl;
				std::cout << this->best.getPenalty() << std::endl;
				return this->best;
			}
		
			auto value = newPopulation.insert(child);
		}
		population = newPopulation;
	}

	this->best.print();
	std::cout << this->best.getNoFaults() << std::endl;
	std::cout << this->best.getPenalty() << std::endl;
	return this->best;
}


