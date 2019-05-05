#include "Node.h"

Node::Node()
{
}

void Node::setAnswer(std::vector<std::pair<int, int>> answer)
{
	this->answers = answer;
}

void Node::addAnswer(std::pair<int, int> answer)
{
	this->answers.push_back(answer);
}

std::vector<std::pair<int, int>> Node::getAnswer()
{
	return this->answers;
}

bool Node::isRoomFree(std::pair<int, int> slot)
{
	for (size_t i = 0; i < this->answers.size(); i++)
	{
		std::pair<int, int> answer = this->answers.at(i);
		if (answer == slot)
			return true;
	}
	return false;
}
