#pragma once

#include "Utils.h"
#include "Node.h"

#include <vector>

/*
Considering a function with period penalty and room penalty
as X and Y respectively and Z as total penalty
in this case example:
if X = 1 && Y = 2
neighbors will be [1,3],[1,1],[2,2],[0,2]
*/
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
	int getStateValue(Node solution);

	std::vector<Exam> getExams();
	std::vector<Period> getPeriods();
	std::vector<Room> getRooms();

private:

	void read();
	void readExams();
	void readPeriods();
	void readRooms();

};