#include <iostream>
#include <fstream>  
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>  

class DataRoom {

	int nrRooms;
	std::pair<int, int> capacity;
	std::pair<int, int> penalty;

	std::vector<std::pair<int, int>> rooms;

	

public:
	DataRoom();
	DataRoom(int nrRooms, std::pair<int, int> capacity, std::pair<int, int> penalty);

	void readRoomInfo();
	void writeRooms();
	void buildRooms();
};