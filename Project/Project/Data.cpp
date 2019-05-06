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
	this->read();
}

void Data::read()
{
	this->readExams();
	this->readPeriods();
	this->readRooms();
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
				this->rooms.push_back(room);
			}
		}

		if (this->roomsCnt != this->rooms.size()) {
			std::cerr << "Parsing error on " << __func__ << "..." << std::endl;
		}

		input.close();
	}
}

Node Data::generateInitialState()
{
	Node node;
	std::random_device rd;
	std::mt19937 mt(rd());
	for (size_t i = 0; i < 5; i++)
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
