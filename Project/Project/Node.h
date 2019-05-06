#pragma once

#include <vector>
#include <utility>

class Node {

	/*
	pair<period, room>
	*/
	std::vector<std::pair<int, int>> answers;

public:

	Node();

	void setAnswers(std::vector<std::pair<int, int>> answers);
	void addAnswer(std::pair<int, int> answer);
	void setAnswer(int index, int fst, int snd);

	std::pair<int, int> getAnswer(int index) const;
	unsigned int getAnswersSize() const;
	bool isRoomFree(std::pair<int, int> slot);

	void print();
};

bool operator==(const Node& lhs, const Node& rhs);
bool operator!=(const Node& lhs, const Node& rhs);