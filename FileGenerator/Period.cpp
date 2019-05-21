#include "Period.h"

Period::Period(int day, int month, int year, int hour, int duration, int penalty)
{
	this->day = day;
	this->month = month;
	this->year = year;
	this->hour = hour;
	this->duration = duration;
	this->penalty = penalty;
}
