#pragma once

#include "Utils.h"
#include "Node.h"
#include <vector>

class Data {
	int examsCnt;
	int periodsCnt;
	int roomsCnt;

	std::vector<Exam> exams;
	std::vector<Period> periods;
	std::vector<Room> rooms;

public:

	Data();
	Node generateInitialState();

private:

	void read();
	void readExams();
	void readPeriods();
	void readRooms();

};