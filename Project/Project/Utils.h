#pragma once

#include <string>

class Time {

	int hours;
	int minutes;
	int seconds;

public:
	Time();
	Time(std::string time);
	Time(int hour, int minutes, int seconds);
	int getHour();
	int getMinutes();
	int getSeconds();

};

class Date {

	int day;
	int month;
	int year;

public:
	Date();
	Date(std::string date);
	Date(int day, int month, int year);
	int getDay();
	int getMonth();
	int getYear();

};

class Exam {

	int duration;
	int studentsCnt;

public:
	Exam(int duration, int sCnt);
	int getDuration();
	int getStudentsCnt();

};

class Period {

	Date date;
	Time time;
	int duration;
	int penalty;

public:
	Period(Date date, Time time, int duration, int penalty);
	Date getDate();
	Time getTime();
	int getDuration();
	int getPenalty();

};

class Room {

	int capacity;
	int penalty;

public:
	Room(int capacity, int penalty);
	int getCapacity();
	int getPenalty();

};



