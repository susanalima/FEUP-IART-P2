#include "Data.h"

#include <random>
#include <iostream>

Data::Data()
{
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
	//TODO
}

void Data::readPeriods()
{
	//TODO
}

void Data::readRooms()
{
	//TODO
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
