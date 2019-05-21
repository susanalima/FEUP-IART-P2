#include <iostream>
#include <fstream>  
#include <random>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>  
#include "Period.h"

class DataPeriod {

	int nrPeriods;
	std::pair<int, int> day;
	std::pair<int, int> month;
	int year;
	std::pair<int, int> hour;
	std::pair<int, int> duration;
	std::pair<int, int> penalty;
	std::vector<Period> periods;

public:
	DataPeriod();
	DataPeriod(int nrPeriods, std::pair<int, int> day, std::pair<int, int> month, int year, 
		std::pair<int, int> hour, std::pair<int, int> duration, std::pair<int, int> penalty);

	void readPeriodInfo();
	void writePeriods();
	void buildPeriods();
};