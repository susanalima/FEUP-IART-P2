#include "Data.h"

#include <random>
#include <iostream>
#include <fstream>
#include <sstream>

Data::Data()
{
	this->examsCnt = 0;
	this->periodsCnt = 0;
	this->roomsCnt = 0;
	this->maxPeriodPenalty = 0;
	this->maxRoomPenalty = 0;

	this->read();

	std::sort(this->periods.begin(), this->periods.end());
	std::sort(this->rooms.begin(), this->rooms.end());

	buildPeriodDays();
	buildIncompatibilitiesTable();
}

void Data::buildPeriodDays() {
	for (int i = 0; i < this->periods.size(); i++) {
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
	for (int i = 0; i < this->exams.size(); i++) {
		Exam exam1 = this->exams.at(i);
		for (int j = i + 1; j < this->exams.size(); j++) {
			Exam exam2 = this->exams.at(j);
			nrOverlaps = exam1.getOverlappingStudents(&exam2).size();
			this->incompatibilitiesTable.insert(std::pair<std::pair<int, int>, int>(std::pair<int, int>(i, j), nrOverlaps));
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
	std::ifstream input(this->exams_input);
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
	std::ifstream input(this->periods_input);
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
				this->maxPeriodPenalty += penalty;
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
	std::ifstream input(this->rooms_input);
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
				this->maxRoomPenalty += penalty;
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
	std::ifstream input(this->roomConstraints_input);
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
	std::ifstream input(this->periodConstraints_input);
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
			}
		}

		input.close();
	}
}

void Data::readInstWeights()
{
	std::string line;
	std::ifstream input(this->institutionalWeightings_input);
	if (input.good())
	{
		if (getline(input, line)) {
		}

	
		int twoInRow, twoInDay, periodSpread, nonMixedDurations, nrExams, nrPeriods, penalty;
		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			int p1,p2,p3, cnt = 0;
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

int Data::getMaxPeriodPenalty() const
{
	return this->maxPeriodPenalty;
}

int Data::getMaxRoomPenalty() const
{
	return this->maxRoomPenalty;
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

std::map<std::string, std::vector<int>> Data::getPeriodDays() const
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

