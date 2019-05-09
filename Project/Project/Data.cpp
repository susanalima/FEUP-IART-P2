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

			int duration, students, cnt = 0;
			while (getline(lineInput, cell, ','))
			{
				if (cnt == 0)
					duration = std::stoi(cell);
				else if(cnt == 1)
					students = std::stoi(cell);
				cnt++;
			}

			if (cnt == 2) {
				Exam exam = Exam(duration, students);
				this->exams.push_back(exam);
			}
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
				Period period = Period(date, time, duration, penalty);
				this->maxPeriodPenalty += penalty;
				this->periods.push_back(period);
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
				Room room = Room(capacity, penalty);
				this->maxRoomPenalty += penalty;
				this->rooms.push_back(room);
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

std::vector<Exam> Data::getExams()
{
	return this->exams;
}

std::vector<Period> Data::getPeriods()
{
	return this->periods;
}

std::vector<Room> Data::getRooms()
{
	return this->rooms;
}

int Data::getExamsCnt()
{
	return this->examsCnt;
}

int Data::getPeriodsCnt()
{
	return this->periodsCnt;
}

int Data::getRoomsCnt()
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
