#pragma once

#include <vector>
#include <utility>

class Node {

	std::vector<std::pair<int, int>> answers;

public:

	Node();

	void setAnswer(std::vector<std::pair<int, int>> answers);
	void addAnswer(std::pair<int, int> answer);

	std::vector<std::pair<int, int>> getAnswer();
	bool isRoomFree(std::pair<int, int> slot);
};