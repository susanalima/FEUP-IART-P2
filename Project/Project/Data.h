#pragma once

#include "Utils.h"
#include "Node.h"

#include <vector>

class Data {

	const std::string exams_input = "exams.txt";
	const std::string periods_input = "periods.txt";
	const std::string rooms_input = "rooms.txt";

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