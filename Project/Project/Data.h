#pragma once

#include "Utils.h"
#include "Node.h"

#include <vector>
#include <map>


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
	const std::string roomConstraints_input = "roomConstraints.txt";
	const std::string periodConstraints_input = "periodConstraints.txt";
	const std::string institutionalWeightings_input = "institutionalWeightings.txt";

	int examsCnt;
	int periodsCnt;
	int roomsCnt;

	std::string folderLoc;

	std::vector<Exam> exams;
	std::vector<Period> periods;
	std::vector<Room> rooms;

	std::map<int, std::string> roomConstraints;
	std::multimap<int, std::pair<int, std::string>> periodConstraints;
	
	std::map<std::string, std::vector<int>, dateComparator> periodDays; 

	InstitutionalWeightings instWeights;

	std::map<std::pair<int, int>, int> incompatibilitiesTable;

	std::map<int, std::vector<int>> periodExams; 

	std::map<int, std::vector<int>> periodIncompatibilities;

public:

	Data();
	Data(std::string folderLoc);
	Node generateInitialState();
	int getStateValue(Node solution);

	std::vector<Exam> getExams() const;
	std::vector<Period> getPeriods() const;
	std::vector<Room> getRooms() const;

	int getExamsCnt() const;
	int getPeriodsCnt() const;
	int getRoomsCnt() const;

	std::string getExamRoomConstraint(int exam) const;
	std::multimap<int, std::pair<int, std::string>> getPeriodConstraints() const;
	InstitutionalWeightings getInstWeights() const;
	std::map<std::string, std::vector<int>, dateComparator> getPeriodDays() const;

	int getExamsOverlaps(int examIndex1, int examIndex2) const;

	void printNodeInfo(Node* node);

	void evaluateSolution(Node* solution);

	void buildDisplay(Node *node);

private:

	void read();
	void readExams();
	void readPeriods();
	void readRooms();
	void readRoomContraints();
	void readPeriodContraints();
	void readInstWeights();
	void buildPeriodDays();
	void buildIncompatibilitiesTable();
	void buildPeriodExams(Node *solution);

	void buildPeriodIncompatibilities();

	int applyPeriodHardConstraints(int index, std::vector<std::pair<int, int>>* schedule, Period* period, Exam* exam, int periodIndex);
	int applyGeneralHardConstraints(int index, std::map<std::pair<int, int>, int>* examSlot, std::vector<std::pair<int, int>>* schedule, Exam* exam, Period* period, Room* room);

};