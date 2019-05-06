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
	int studentsCnt;

public:
	Exam(int duration, int sCnt);
	int getDuration() const;
	int getStudentsCnt() const;

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


