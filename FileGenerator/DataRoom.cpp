#include "DataRoom.h"




DataRoom::DataRoom()
{
	this->readRoomInfo();
	this->buildRooms();
}

DataRoom::DataRoom(int nrRooms, std::pair<int, int> capacity, std::pair<int, int> penalty)
{
	this->nrRooms = nrRooms;
	this->capacity = capacity;
	this->penalty = penalty;

	this->buildRooms();
}

void DataRoom::readRoomInfo()
{
	std::string line;
	std::ifstream input("infoRoom.txt");
	if (input.good())
	{
		if (getline(input, line)) {
			this->nrRooms = std::stoi(line);
		}

		int gCnt = 0;

		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;


			int minCapacity, maxCapacity, minPenalty, maxPenalty, cnt = 0;
			while (getline(lineInput, cell, ','))
			{
				switch (cnt)
				{
				case 0:
					minCapacity = stoi(cell);
					break;
				case 1:
					maxCapacity = stoi(cell);
					break;
				case 2:
					minPenalty = stoi(cell);
					break;
				case 3:
					maxPenalty = stoi(cell);
					break;
				default:
					break;
				}

				cnt++;
			}

			this->capacity = std::pair<int, int>(minCapacity, maxCapacity);
			this->penalty = std::pair<int, int>(minPenalty, maxPenalty);

			gCnt++;
			if (gCnt == 1)
				break;

		}
		input.close();
	}
}

void DataRoom::writeRooms()
{
	std::ofstream outfile("rooms.txt");
	outfile << "Rooms:" << this->nrRooms << std::endl;
	for (int i = 0; i < this->nrRooms; i++) {
		outfile <<  this->rooms.at(i).first << "," << this->rooms.at(i).second << std::endl;
	}
	outfile.close();

}

void DataRoom::buildRooms()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int rCapacity, rPenalty;
	for (int i = 0; i < this->nrRooms; i++) {
		rCapacity = mt() % (this->capacity.second - this->capacity.first + 1) + this->capacity.first;
		rPenalty = mt() % (this->penalty.second - this->penalty.first + 1) + this->penalty.first;
		this->rooms.push_back(std::pair<int, int>(rCapacity, rPenalty));
	}
}
