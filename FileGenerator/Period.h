#include <random>
#include <iostream>
#include <iterator>     
#include <chrono>

class Period {


public:

	int day;
	int month;
	int year;
	int hour;
	int duration;
	int penalty;

	Period(int day, int month, int year, int hour, int duration, int penalty);


};