#pragma once

#include <string>
#include <vector>

class Time {

	int hours;
	int minutes;
	int seconds;

public:
	Time();
	Time(std::string time);
	Time(int hour, int minutes, int seconds);
	int getHours() const;
	int getMinutes() const;
	int getSeconds() const;

};

class Date {

	int day;
	int month;
	int year;

public:
	Date();
	Date(std::string date);
	Date(int day, int month, int year);
	int getDay() const;
	int getMonth() const;
	int getYear() const;
};

class Exam {

	int duration;
	//int studentsCnt;
	std::vector<int> students;

public:
	Exam(int duration, std::vector<int> students);
	int getDuration() const;
	int getStudentsCnt() const;
	std::vector<int> getStudents() const;

};

class Period {

	Date date;
	Time time;
	int duration;
	int penalty;

public:
	Period(Date date, Time time, int duration, int penalty);
	Date getDate() const;
	Time getTime() const;
	int getDuration() const;
	int getPenalty() const;

};

class Room {

	int capacity;
	int penalty;

public:
	Room(int capacity, int penalty);
	int getCapacity() const;
	int getPenalty() const;

};


class FrontLoad {

	int nrExams;
	int nrPeriods;
	int penalty;

public:
	FrontLoad();
	FrontLoad(int nrExams, int nrPeriods, int penalty);
	int getNrExams() const;
	int getNrPeriods() const;
	int getPenalty() const;

};

class InstitutionalWeightings {

	int twoInRow;
	int twoInDay;
	int periodSpreed;
	int nonMixedDurations;
	FrontLoad frontLoad;

public:
	
	InstitutionalWeightings();
	InstitutionalWeightings(int twoInRow, int twoInDay, int periodSpreed, int nonMixedDurations, int nrExams, int nrPeriods, int penalty);
	int getTwoInRow() const;
	int getTwoInDay() const;
	int getPeriodSpreed() const;
	int getNonMixedDurations() const;
	FrontLoad getFrontLoad() const;
};



bool operator< (const Date& lhs, const Date& rhs);
bool operator> (const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);

bool operator< (const Time& lhs, const Time& rhs);
bool operator> (const Time& lhs, const Time& rhs);
bool operator<=(const Time& lhs, const Time& rhs);
bool operator>=(const Time& lhs, const Time& rhs);
bool operator==(const Time& lhs, const Time& rhs);

bool operator< (const Period& lhs, const Period& rhs);
bool operator> (const Period& lhs, const Period& rhs);
bool operator<=(const Period& lhs, const Period& rhs);
bool operator>=(const Period& lhs, const Period& rhs);
bool operator==(const Period& lhs, const Period& rhs);

bool operator< (const Room& lhs, const Room& rhs);
bool operator> (const Room& lhs, const Room& rhs);
bool operator<=(const Room& lhs, const Room& rhs);
bool operator>=(const Room& lhs, const Room& rhs);
bool operator==(const Room& lhs, const Room& rhs);

bool operator< (const Exam& lhs, const Exam& rhs);
bool operator==(const Exam& lhs, const Exam& rhs);


