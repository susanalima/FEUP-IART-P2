#include "Utils.h"

#include <ctime>
#include <sstream>

Exam::Exam(int duration, int sCnt)
{
	this->duration = duration;
	this->studentsCnt = sCnt;
}

int Exam::getDuration()
{
	return this->duration;
}

int Exam::getStudentsCnt()
{
	return this->studentsCnt;
}

Period::Period(Date date, Time time, int duration, int penalty)
{
	this->date = date;
	this->time = time;
	this->duration = duration;
	this->penalty = penalty;
}

Date Period::getDate()
{
	return this->date;
}

Time Period::getTime()
{
	return this->time;
}

int Period::getDuration()
{
	return this->duration;
}

int Period::getPenalty()
{
	return this->duration;
}

Room::Room(int capacity, int penalty)
{
	this->capacity = capacity;
	this->penalty = penalty;
}

int Room::getCapacity()
{
	return this->capacity;
}

int Room::getPenalty()
{
	return this->penalty;
}

Time::Time()
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	this->hours = ltm.tm_hour;
	this->minutes = ltm.tm_min;
	this->seconds = ltm.tm_sec;
}

Time::Time(std::string time)
{
	std::stringstream input(time);
	std::string cell;

	int cnt = 0;
	while (getline(input, cell, ':'))
	{
		if (cnt == 0)
			this->hours = std::stoi(cell);
		else if (cnt == 1)
			this->minutes = std::stoi(cell);
		else if (cnt == 2)
			this->seconds = std::stoi(cell);
		cnt++;
	}
}

Time::Time(int hour, int minutes, int seconds)
{
	this->hours = hour;
	this->minutes = minutes;
	this->seconds = seconds;
}

int Time::getHour()
{
	return this->hours;
}

int Time::getMinutes()
{
	return this->minutes;
}

int Time::getSeconds()
{
	return this->seconds;
}

Date::Date()
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	this->day = ltm.tm_mday;
	this->month = ltm.tm_mon;
	this->year = ltm.tm_year;
}

Date::Date(std::string date)
{
	std::stringstream input(date);
	std::string cell;

	int cnt = 0;
	while (getline(input, cell, ':'))
	{
		if (cnt == 0)
			this->day = std::stoi(cell);
		else if (cnt == 1)
			this->month = std::stoi(cell);
		else if (cnt == 2)
			this->year = std::stoi(cell);
		cnt++;
	}
}

Date::Date(int day, int month, int year)
{
	this->day = day;
	this->month = month;
	this->year = year;
}

int Date::getDay()
{
	return this->day;
}

int Date::getMonth()
{
	return this->month;
}

int Date::getYear()
{
	return this->year;
}
