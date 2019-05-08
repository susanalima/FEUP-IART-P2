#include "Node.h"

#include <iostream>

Node::Node()
{
}

void Node::setAnswers(std::vector<std::pair<int, int>> answer)
{
	this->answers = answer;
}

void Node::addAnswer(std::pair<int, int> answer)
{
	this->answers.push_back(answer);
}

void Node::setAnswer(int index, int fst, int snd)
{
	std::pair<int, int> pair;
	pair.first = fst;
	pair.second = snd;
	this->answers.at(index) = pair;
}

std::vector<std::pair<int, int>> Node::getAnswers() 
{
	return this->answers;
}

std::pair<int, int> Node::getAnswer(int index) const
{
	return this->answers.at(index);
}

unsigned int Node::getAnswersSize() const
{
	return this->answers.size();
}

bool Node::isRoomFree(std::pair<int, int> slot)
{
	for (size_t i = 0; i < this->answers.size(); i++)
	{
		std::pair<int, int> answer = this->answers.at(i);
		if (answer == slot)
			return false;
	}
	return true;
}

void Node::print()
{
	for (size_t i = 0; i < this->answers.size(); i++)
	{
		std::pair<int, int> p = this->answers.at(i);
		std::cout << p.first << ", " << p.second << std::endl;
	}
}

bool operator==(const Node& lhs, const Node& rhs)
{
	if (lhs.getAnswersSize() == rhs.getAnswersSize()) {
		for (size_t i = 0; i < lhs.getAnswersSize(); i++)
		{
			std::pair<int, int> p1 = lhs.getAnswer(i);
			std::pair<int, int> p2 = rhs.getAnswer(i);

			bool compare = p1.first == p2.first && p1.second == p2.second;
			if (!compare)
				return false;
		}
		return true;
	}
	return false;
}

bool operator!=(const Node& lhs, const Node& rhs)
{
	return !(lhs == rhs);
}


