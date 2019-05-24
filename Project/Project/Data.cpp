#include "Data.h"

#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <iterator>   

Data::Data()
{
	this->examsCnt = 0;
	this->periodsCnt = 0;
	this->roomsCnt = 0;

	this->folderLoc = "";

	this->read();

	std::sort(this->periods.begin(), this->periods.end());
	std::sort(this->rooms.begin(), this->rooms.end());

	buildPeriodDays();
	buildIncompatibilitiesTable();
	buildPeriodIncompatibilities();
}

Data::Data(std::string folderLoc)
{
	this->examsCnt = 0;
	this->periodsCnt = 0;
	this->roomsCnt = 0;

	this->folderLoc = folderLoc;

	this->read();

	std::sort(this->periods.begin(), this->periods.end());
	std::sort(this->rooms.begin(), this->rooms.end());

	buildPeriodDays();
	buildIncompatibilitiesTable();
	buildPeriodIncompatibilities();
}

void Data::buildPeriodDays() {
	for (auto i = 0; i < this->periods.size(); i++) {
		std::string sDate = this->periods.at(i).getDate().getDate();
		auto it = this->periodDays.find(sDate);
		if (it != this->periodDays.end()) {
			it->second.push_back(i);
		}
		else {
			std::vector<int> periods;
			periods.push_back(i);
			periodDays.insert(std::pair<std::string, std::vector<int>>(sDate, periods));
		}
	}
}

void Data::buildIncompatibilitiesTable()
{
	int nrOverlaps;
	for (auto i = 0; i < this->exams.size(); i++) {
		Exam exam1 = this->exams.at(i);
		for (auto j = i + 1; j < this->exams.size(); j++) {
			Exam exam2 = this->exams.at(j);
			nrOverlaps = exam1.getOverlappingStudents(&exam2).size();
			this->incompatibilitiesTable.insert(std::pair<std::pair<int, int>, int>(std::pair<int, int>(i, j), nrOverlaps));
			if (nrOverlaps > 0) {
				this->exams.at(i).addIncompatibleExam(j);
				this->exams.at(j).addIncompatibleExam(i);
			}
		}
	}
	/*for (auto it = this->incompatibilitiesTable.begin(); it != this->incompatibilitiesTable.end(); it++)
	{
		std::cout << it->first.first << ":" << it->first.second << "-" << it->second << std::endl;
	}*/
}

void Data::buildPeriodExams(Node* solution)
{
	this->periodExams.clear();
	std::pair<int, int> answer;
	int periodIndex;
	for (auto i = 0; i < solution->getAnswersSize(); i++) {
		answer = solution->getAnswer(i);
		periodIndex = answer.first;
		auto it = this->periodExams.find(periodIndex);
		if (it != this->periodExams.end()) {
			it->second.push_back(i);
		}
		else {
			std::vector<int> exams;
			exams.push_back(i);
			periodExams.insert(std::pair<int, std::vector<int>>(periodIndex, exams));
		}
	}
}

void Data::buildPeriodIncompatibilities()
{
	for (auto i = 0; i < this->periods.size(); i++) {
		Period p1 = this->periods.at(i);
		for (auto j = i + 1; j < this->periods.size(); j++) {
			Period p2 = this->periods.at(j);
			if (p1.getDate() == p2.getDate()) {
				if (p1.getEndTime() > p2.getTime() && p1.getTime() <= p2.getTime()) {
					auto it = this->periodIncompatibilities.find(i);
					if (it != this->periodIncompatibilities.end()) {
						it->second.push_back(j);
					}
					else {
						std::vector<int> periods;
						periods.push_back(j);
						this->periodIncompatibilities.insert(std::pair<int, std::vector<int>>(i, periods));
					}
				}
			}	
		}
	}
}


void Data::read()
{
	this->readExams();
	this->readPeriods();
	this->readRooms();
	this->readRoomContraints();
	this->readPeriodContraints();
	this->readInstWeights();
}

void Data::readExams()
{
	std::string line;
	std::ifstream input(this->folderLoc + this->exams_input);
	if (input.good()) 
	{
		if (getline(input, line)) {
			line = line.substr(line.find(':') + 1);
			this->examsCnt = std::stoi(line);
		}

		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			std::vector<int> students;
			int duration, cnt = 0;
			while (getline(lineInput, cell, ','))
			{
				if (cnt == 0)
					duration = std::stoi(cell);
				else
					students.push_back(std::stoi(cell));
				cnt++;
			}


			Exam exam = Exam(duration, students);
			this->exams.push_back(exam);
			
		}

		if (this->examsCnt != this->exams.size()) {
			std::cerr << "Parsing error on " << __func__ << "..." << std::endl;
		}

		input.close();
	}
}

void Data::readPeriods()
{
	std::string line;
	std::ifstream input(this->folderLoc + this->periods_input);
	if (input.good())
	{
		if (getline(input, line)) {
			line = line.substr(line.find(':') + 1);
			this->periodsCnt = std::stoi(line);
		}

		int id = 0;
		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			Date date;
			Time time;
			int duration, penalty, cnt = 0;
			while (getline(lineInput, cell, ','))
			{
				if (cnt == 0)
					date = Date(cell);
				else if (cnt == 1)
					time = Time(cell);
				else if (cnt == 2)
					duration = std::stoi(cell);
				else if (cnt == 3)
					penalty = std::stoi(cell);
				cnt++;
			}

			if (cnt == 4) {
				Period period = Period(date, time, duration, penalty, id);
				this->periods.push_back(period);
				id++;
			}
		}

		if (this->periodsCnt != this->periods.size()) {
			std::cerr << "Parsing error on " << __func__ << "..." << std::endl;
		}

		input.close();
	}
}

void Data::readRooms()
{
	std::string line;
	std::ifstream input(this->folderLoc + this->rooms_input);
	if (input.good())
	{
		if (getline(input, line)) {
			line = line.substr(line.find(':') + 1);
			this->roomsCnt = std::stoi(line);
		}

		int id = 0;
		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			int capacity, penalty, cnt = 0;
			while (getline(lineInput, cell, ','))
			{
				if (cnt == 0)
					capacity = std::stoi(cell);
				else if (cnt == 1)
					penalty = std::stoi(cell);
				cnt++;
			}

			if (cnt == 2) {
				Room room = Room(capacity, penalty, id);
				this->rooms.push_back(room);
				id++;
			}
		}

		if (this->roomsCnt != this->rooms.size()) {
			std::cerr << "Parsing error on " << __func__ << "..." << std::endl;
		}

		input.close();
	}
}

void Data::readRoomContraints()
{
	std::string line;
	std::ifstream input(this->folderLoc + this->roomConstraints_input);
	if (input.good())
	{

		if (getline(input, line)) {
		}

		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			int room, cnt = 0;
			std::string constraint;
			while (getline(lineInput, cell, ','))
			{
				if (cnt == 0)
					room = std::stoi(cell);
				else if (cnt == 1)
					constraint = cell;
				cnt++;
			}

			if (cnt == 2) {
				auto end_pos = std::remove(constraint.begin(), constraint.end(), ' ');
				constraint.erase(end_pos, constraint.end());
				this->roomConstraints.insert(std::pair<int, std::string>(room, constraint));
			}
		}

		input.close();
	}
}

void Data::readPeriodContraints()
{
	std::string line;
	std::ifstream input(this->folderLoc + this->periodConstraints_input);
	if (input.good())
	{
		if (getline(input, line)) {
		}

		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			int exam1, exam2, cnt = 0;
			std::string constraint;
			while (getline(lineInput, cell, ','))
			{
				if (cnt == 0)
					exam1 = std::stoi(cell);
				else if (cnt == 1)
					constraint = cell;
				else if (cnt == 2)
					exam2 = std::stoi(cell);
			
				cnt++;
			}

			if (cnt == 3) {
				auto end_pos = std::remove(constraint.begin(), constraint.end(), ' ');
				constraint.erase(end_pos, constraint.end());
				this->periodConstraints.insert(std::pair<int, std::pair<int, std::string>>(exam1, std::pair<int, std::string>(exam2, constraint)));
				this->exams.at(exam1).addPeriodConstraint(exam2, constraint);
				if(constraint.compare("AFTER") != 0)
					this->exams.at(exam2).addPeriodConstraint(exam1, constraint);
			}
		}
		input.close();
	}
}

void Data::readInstWeights()
{
	std::string line;
	std::ifstream input(this->folderLoc + this->institutionalWeightings_input);
	if (input.good())
	{
		if (getline(input, line)) {
		}

	
		int twoInRow, twoInDay, periodSpread, nonMixedDurations, nrExams, nrPeriods, penalty;
		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			int p1 = 0,p2 = 0,p3 = 0, cnt = 0;
			std::string constraint;
			while (getline(lineInput, cell, ','))
			{
				if (cnt == 0)
					constraint = cell;
				else if (cnt == 1)
					p1 = std::stoi(cell);
				else if (cnt == 2)
					p2 = std::stoi(cell);
				else if (cnt == 3)
					p3 = std::stoi(cell);

				cnt++;
			}
			
			if (constraint.compare("TWOINAROW") == 0) {
				twoInRow = p1;
			}
			else if (constraint.compare("TWOINADAY") == 0) {
				twoInDay = p1;
			}
			else if (constraint.compare("PERIODSPREAD") == 0) {
				periodSpread = p1;
			}
			else if (constraint.compare("NONMIXEDDURATIONS") == 0) {
				nonMixedDurations = p1;
			}
			else if (constraint.compare("FRONTLOAD") == 0) {
				nrExams = p1;
				nrPeriods = p2;
				penalty = p3;
				this->instWeights = InstitutionalWeightings(twoInRow, twoInDay, periodSpread, nonMixedDurations, nrExams, nrPeriods, penalty);
			}	
		}

		input.close();
	}
}

Node Data::generateInitialState()
{
	Node node;
	std::random_device rd;
	std::mt19937 mt(rd());
	for (size_t i = 0; i < this->examsCnt; i++)
	{
		std::pair<int, int> pair;
		bool available = false;
		while (!available) {
			int period = mt() % this->periodsCnt;
			int room = mt() % this->roomsCnt;
			pair = std::pair<int, int>(period, room);
			available = node.isRoomFree(pair);
		}
		node.addAnswer(pair);
	}
	return node;
}

int Data::getStateValue(Node solution)
{
	int value = 0;
	for (size_t i = 0; i < solution.getAnswersSize(); i++)
	{
		std::pair<int, int> answer = solution.getAnswer(i);
		value += this->periods.at(answer.first).getPenalty();
		value += this->rooms.at(answer.second).getPenalty();
	}
	return value;
}

std::vector<Exam> Data::getExams() const
{
	return this->exams;
}

std::vector<Period> Data::getPeriods() const
{
	return this->periods;
}

std::vector<Room> Data::getRooms() const
{
	return this->rooms;
}

int Data::getExamsCnt() const
{
	return this->examsCnt;
}

int Data::getPeriodsCnt() const
{
	return this->periodsCnt;
}

int Data::getRoomsCnt() const
{
	return this->roomsCnt;
}

std::string Data::getExamRoomConstraint(int exam) const
{
	std::string constraint = "";
	auto it = this->roomConstraints.find(exam);
	if (it != this->roomConstraints.end()) {
		constraint = it->second;
	}
	return constraint;
}

std::multimap<int, std::pair<int, std::string>> Data::getPeriodConstraints() const
{
	return this->periodConstraints;
}

InstitutionalWeightings Data::getInstWeights() const
{
	return this->instWeights;
}

std::map<std::string, std::vector<int>, dateComparator> Data::getPeriodDays() const
{
	return this->periodDays;
}

int Data::getExamsOverlaps(int examIndex1, int examIndex2) const
{
	auto it = this->incompatibilitiesTable.find(std::pair<int, int>(examIndex1, examIndex2));
	if (it != this->incompatibilitiesTable.end()) {
		return it->second;
	}
	else
		return getExamsOverlaps(examIndex2, examIndex1);
}


//prints de period and room ids instead of the indexes (ids are in the order of the file)
void Data::printNodeInfo(Node* node)
{
	std::vector<std::pair<int, int>> answers = node->getAnswers();
	int periodIndex, roomIndex;
	for (int i = 0; i < answers.size(); i++) {
		periodIndex = answers.at(i).first;
		roomIndex = answers.at(i).second;
		std::cout << this->periods.at(periodIndex).getId() << ", " << this->rooms.at(roomIndex).getId() << std::endl;
	}
	std::cout << "nr of faults : " << node->getNoFaults() << std::endl;
	std::cout << "penalty : " << node->getPenalty() << std::endl;
}


void Data::evaluateSolution(Node* solution)
{
	int penalty = 0, noFaults = 0, roomIndex, periodIndex;
	std::string roomConstraint, periodConstraint;

	std::vector<std::pair<int, int>> schedule = solution->getAnswers();
	std::vector<Exam>  examsSorted = this->exams;

	std::map<std::pair<int, int>, int> examSlot; //key : periodIndex, roomIndex, value: roomOcSpace
	//key : periodIndex
	//value : pair : set -> key = exam duration ; vector : exam index
	std::map<int, std::pair<std::set<int>, std::vector<int>>> periodInfo;
	std::map<std::string, std::vector<int>> examDays;

	int flNrExams = this->instWeights.getFrontLoad().getNrExams();
	int flNrPeriods = this->instWeights.getFrontLoad().getNrExams();
	int flPenalty = this->instWeights.getFrontLoad().getPenalty();

	std::sort(examsSorted.begin(), examsSorted.end()); //sorted by student count

	for (int i = 0; i < schedule.size(); i++) {

		Exam exam = this->exams.at(i);

		periodIndex = schedule.at(i).first;
		Period period = this->periods.at(periodIndex);

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
		Room room = this->rooms.at(roomIndex);

		noFaults += applyGeneralHardConstraints(i, &examSlot, &schedule, &exam, &period, &room);
		noFaults += applyPeriodHardConstraints(i, &schedule, &period, &exam, periodIndex);

		penalty += room.getPenalty(); //penalty of using the room
		penalty += period.getPenalty(); //penalty of using the period

		//Larger Exams towards the beginning of the examination session constraint
		auto it = find(examsSorted.begin(), examsSorted.end(), exam);
		int pos = distance(examsSorted.begin(), it);

		if (pos >= flNrExams && periodIndex >= flNrPeriods)
			penalty += flPenalty;
	}

	int periodPenalty = 0, currentPeriodIndex, overlappingNo;

	//percorre todos os periodos e ve para cada 1
	//*a cena das mixed durations
	//*os periodos no mesmo dia
	//*para cada exame do periodo ve as colisoes com os outros exames do mesmo periodo
	//*para cada exame do periodo ve as colisoes com os exames dos outros periodos do mesmo dia (os periodos depois dele)

	for (auto it = periodInfo.begin(); it != periodInfo.end(); it++) {

		currentPeriodIndex = it->first;

		//Mixed Durations constraint
		penalty += (it->second.first.size() - 1) * this->instWeights.getNonMixedDurations();

		std::vector<int> periodExams = it->second.second; //todos os exames do periodo de index it->first

		std::string sDate = this->periods.at(currentPeriodIndex).getDate().getDate(); //dia do periodo 
		std::vector<int> sameDayPeriods = this->periodDays[sDate]; //todos os periodos daquele dia
		std::vector<int> collidingPeriods;
		collidingPeriods.clear();
		auto tmp = this->periodIncompatibilities.find(currentPeriodIndex);
		if(tmp != this->periodIncompatibilities.end())
			collidingPeriods = this->periodIncompatibilities.find(currentPeriodIndex)->second;

		int sameDayPeriodIndex;  //index do periodo daquele dia
		std::vector<int> sameDayExams;

		int e1, e2;
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
			if (iter == periodInfo.end())
				continue;

			sameDayExams = iter->second.second;

			for (int k = 0; k < periodExams.size(); k++) {
				e1 = periodExams.at(k);
				for (int j = 0; j < sameDayExams.size(); j++) {
					e2 = sameDayExams.at(j);
					overlappingNo = getExamsOverlaps(e1, e2);
					penalty += overlappingNo * periodPenalty;
				}
			}
		}


		for (auto k = 0; k < collidingPeriods.size(); k++) {
			auto tI = periodInfo.find(k);
			if (tI == periodInfo.end())
				continue;
			std::vector<int> tExams =  tI->second.second;
			for (auto i = 0; i < periodExams.size(); i++) {
				e1 = periodExams.at(i);

				//ve as colisoes de todos os exames de periodos coincidentes
				for (auto j = 0; j < tExams.size(); j++) {
					e2 = tExams.at(j);
					if (getExamsOverlaps(e1, e2) != 0) {
						noFaults++;
					}
				}
			}
		}

		for (auto i = 0; i < periodExams.size(); i++) {
			e1 = periodExams.at(i);

			//ve as colisoes de todos os exames de um periodo
			for (int j = i + 1; j < periodExams.size(); j++) {
				e2 = periodExams.at(j);
				if (getExamsOverlaps(e1, e2) != 0) {
					noFaults++;
				}
			}

			//Period Spread constraint
			for (auto j = currentPeriodIndex + 1; j < periods.size(); j++) {

				auto iter = periodInfo.find(j);
				if (iter == periodInfo.end())
					continue;
				for (int k = 0; k < iter->second.second.size(); k++) {
					e2 = iter->second.second.at(k);
					overlappingNo = getExamsOverlaps(e1, e2);
					penalty += overlappingNo;
				}
				if (j >= this->instWeights.getPeriodSpreed())
					break;
			}
		}
	}

	solution->setNoFaults(noFaults);
	solution->setPenalty(penalty);
}

int Data::applyPeriodHardConstraints(int index, std::vector<std::pair<int, int>> * schedule, Period * period, Exam * exam, int periodIndex)
{
	std::string periodConstraint;
	std::multimap<int, std::pair<int, std::string>> periodConstraints = getPeriodConstraints();
	auto  ret = periodConstraints.equal_range(index);
	int exam2Index, noFaults = 0, period2Index;
	for (auto it = ret.first; it != ret.second; ++it) {
		periodConstraint = it->second.second;
		exam2Index = it->second.first;
		period2Index = schedule->at(exam2Index).first;

		if (periodConstraint.compare("AFTER") == 0) {
			Period period2 = this->periods.at(period2Index);
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
				if (getExamsOverlaps(index, exam2Index) == 0)
					noFaults++;
			}
		}
	}

	return noFaults;
}


int Data::applyGeneralHardConstraints(int index, std::map<std::pair<int, int>, int> * examSlot, std::vector<std::pair<int, int>> * schedule, Exam * exam, Period * period, Room * room)
{
	int noFaults = 0;
	std::string roomConstraint = getExamRoomConstraint(index);

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
	if (room->getCapacity() < it->second)
		noFaults++;  //penalty of exceding the room's capacity

	return noFaults;
}


void Data::buildDisplay(Node* node, std::string aName)
{
	buildPeriodExams(node);
	std::ofstream outfile("schedule.html");
	outfile << "<!DOCTYPE html>" << std::endl;
	outfile << "<html lang=\"en\">" << std::endl;
	outfile << "<head>" << std::endl;
	outfile << "<meta charset=\"UTF - 8\" />" << std::endl;
	outfile << "<meta name=\"viewport\" content=\"width = device - width, initial - scale = 1.0\" />" << std::endl;
	outfile << "<meta http-equiv=\"X - UA - Compatible\" content=\"ie = edge\" />" << std::endl;
	outfile << "<link rel=\"stylesheet\" type=\"text" << "/css\" href=\"main.css\" media=\"screen\" />" << std::endl;
	outfile << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\"crossorigin = \"anonymous\" />" << std::endl;
	outfile << "<title>Document</title>" << std::endl;
	outfile << "</head>" << std::endl;
	outfile << "<body>" << std::endl;
	outfile << "<h2 class=\" title\">" << aName << "</h2>" << std::endl;
	outfile << "<h5 class=\"subtitle\"> Hard Contraints violations number: " << node->getNoFaults() << "</h5>" << std::endl;
	outfile << "<h5 class=\"subtitle\"> Soft Constraints total penalty: " << node->getPenalty() << "</h5>" << std::endl;
	outfile << "<table class = \"table table-bordered borderless main\">" << std::endl;
	for (auto it = this->periodDays.begin(); it != this->periodDays.end(); it++) {
		outfile << "<td class=\"borderless\">" << std::endl;
		outfile << "<table>" << std::endl;
		outfile << "<tr>" << std::endl;
		outfile << "<td></td>" << std::endl;
		outfile << "<td>" << it->first << "</td>" << std::endl;
		outfile << "</tr>" << std::endl;
		for (auto pI = 0; pI < it->second.size(); pI++) {
			outfile << "<tr>" << std::endl;
			outfile << "<td>" << this->periods.at(it->second.at(pI)).getTime().getTime() << " - " << this->periods.at(it->second.at(pI)).getEndTime().getTime();
			outfile << " (" << this->periods.at(it->second.at(pI)).getId() << ")</td>" << std::endl;
			outfile << "<td>" << std::endl;
			auto pIter = this->periodExams.find(it->second.at(pI));
			if (pIter == this->periodExams.end()) {
				continue;
			}
			int examIndex, roomIndex;
			for (auto eI = 0; eI < pIter->second.size(); eI++) {
				examIndex = pIter->second.at(eI);
				roomIndex = node->getAnswer(examIndex).second;
				Exam exam = this->exams.at(examIndex);
				outfile << "<div class=\"modalBtn\"><a data-toggle=\"modal\" data-target=\"#e" << examIndex << "Modal\">" << "exam:" << examIndex;
				outfile << " room:" << this->rooms.at(roomIndex).getId() << "</a></div>" << std::endl;
				outfile << "<div class = \"modal fade\" id = \"e" << examIndex << "Modal\" tabindex = \"-1\" role = \"dialog\" aria-labelledby=\"e" << examIndex << "ModalLabel\" aria-hidden = \"true\">" << std::endl;
				outfile << "<div class = \"modal-dialog\" role = \"document\">" << std::endl;
				outfile << "<div class = \"modal-content\">" << std::endl;
				outfile << "<div class = \"modal-header\">" << std::endl;
				outfile << "<h5 class = \"modal-title\"id = \"e" << examIndex << "ModalLabel\">" << "exam " << examIndex << " in room " << this->rooms.at(roomIndex).getId() << "</h5>" << std::endl;
				outfile << "<button type = \"button\" class = \"close\" data-dismiss = \"modal\" aria-label = \"Close\">" << std::endl;
				outfile << "<span aria - hidden = \"true\"> & times; </span>" << std::endl;
				outfile << "</button>" << std::endl;
				outfile << "</div>" << std::endl;
				outfile << "<div class = \"modal-body\">" << std::endl;
				outfile << "<ul class = \"list-group list-group-flush\">" << std::endl;
				outfile << "<li class = \"list-group-item\">exam duration: "<< exam.getDuration() << " minutes </li>" << std::endl;
				outfile << "<li class = \"list-group-item\">exam number of students: "<< exam.getStudentsCnt() << "</li>" << std::endl;
				outfile << "<li class = \"list-group-item\">exams with overlapping students: ";
				if (exam.getIncompatibleExams().size() == 0)
					outfile << "none";
				else {
					for (auto i = 0; i < exam.getIncompatibleExams().size(); i++) {
						outfile << exam.getIncompatibleExams().at(i);
						if (i < exam.getIncompatibleExams().size() - 1)
							outfile << ", ";
					}
				}
				outfile << "</li>" << std::endl;
				outfile << "<li class = \"list-group-item\">room capacity: " << this->rooms.at(roomIndex).getCapacity() << " students </li>" << std::endl;
				outfile << "<li class = \"list-group-item\">room penalty: " << this->rooms.at(roomIndex).getPenalty() << "</li>" << std::endl;
				auto rE = this->roomConstraints.find(examIndex);
				if (rE != this->roomConstraints.end() || exam.getPeriodConstraints().size() > 0) {
					outfile << "<li class = \"list-group-item\">exam constraints: " << std::endl;
					outfile << "<ol>" << std::endl;
					if (rE != this->roomConstraints.end())
						outfile << "<li class = \"list-group-item\">room exclusive</li>" << std::endl;

					for (auto m = 0; m < exam.getPeriodConstraints().size(); m++) {
						outfile << "<li class = \"list-group-item\">" << exam.getPeriodConstraints().at(m).second << " " << exam.getPeriodConstraints().at(m).first << "</li>" << std::endl;
					}

					outfile << "</ol>" << std::endl;
					outfile << "</li>" << std::endl;
				}
				outfile << "</ul>" << std::endl;
				outfile << "</div>" << std::endl;
				outfile << "<div class = \"modal-footer\">" << std::endl;
				outfile << "<button type = \"button\" class = \"btn btn-secondary\" data-dismiss = \"modal\">Close</button>" << std::endl;
				outfile << "</div>" << std::endl;
				outfile << "</div>" << std::endl;
				outfile << "</div>" << std::endl;
				outfile << "</div>" << std::endl;
			}
			outfile << "</td>" << std::endl;
			outfile << "</tr>" << std::endl;
		}
		outfile << "</table>" << std::endl;
		outfile << "</td>" << std::endl;
	}
	outfile << "</table>" << std::endl;
	outfile << "</body>" << std::endl;
	outfile << "<script src=\"https://code.jquery.com/jquery-3.3.1.slim.min.js\" integrity=\"sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo\" crossorigin=\"anonymous\"></script>" << std::endl;
	outfile << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js\" integrity=\"sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1\" crossorigin=\"anonymous\"></script>" << std::endl;
	outfile << "<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js\" integrity=\"sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM\" crossorigin=\"anonymous\"></script>" << std::endl;
	outfile << "</html>" << std::endl;
	outfile.close();
}
