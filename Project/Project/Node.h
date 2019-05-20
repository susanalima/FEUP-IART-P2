#pragma once

#include <vector>
#include <utility>

class Node {

	/*
	pair<period, room>
	*/
	std::vector<std::pair<int, int>> answers;

	int penalty;
	int noFaults;

	int value;

public:

	Node();

	void setAnswers(std::vector<std::pair<int, int>> answers);
	void addAnswer(std::pair<int, int> answer);
	void setAnswer(int index, int fst, int snd);

	std::vector<std::pair<int, int>> getAnswers() ;
	std::pair<int, int> getAnswer(int index) const;
	unsigned int getAnswersSize() const;
	bool isRoomFree(std::pair<int, int> slot);

	void setPenalty(int penalty);
	void setNoFaults(int noFaults);

	void incPenalty(int inc);
	void incNoFaults(int inc);

	int getPenalty() const;
	int getNoFaults() const;
	int getValue() const;

	void print();
	void updateValue();
};

bool operator==(const Node& lhs, const Node& rhs);
bool operator!=(const Node& lhs, const Node& rhs);
bool operator<(const Node& lhs, const Node& rhs);
bool operator>(const Node& lhs, const Node& rhs);
bool operator<=(const Node& lhs, const Node& rhs);

