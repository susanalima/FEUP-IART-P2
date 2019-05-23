#pragma once

#include <string>
#include <vector>

class Time {

	int hours;
	int minutes;
	int seconds;
	std::string time_;

public:
	Time();
	Time(std::string time);
	Time(int hour, int minutes, int seconds);
	int getHours() const;
	int getMinutes() const;
	int getSeconds() const;
	std::string getTime() const;
};

class Date {

	int day;
	int month;
	int year;
	std::string date;

public:
	Date();
	Date(std::string date);
	Date(int day, int month, int year);
	int getDay() const;
	int getMonth() const;
	int getYear() const;
	std::string getDate() const;
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

	std::vector<int> getOverlappingStudents(Exam* exam);


};

class Period {

	Date date;
	Time time;
	int duration;
	int penalty;
	int id;

public:
	Period(Date date, Time time, int duration, int penalty, int id);
	Date getDate() const;
	Time getTime() const;
	int getDuration() const;
	int getPenalty() const;
	int getId() const;
};

class Room {

	int id;
	int capacity;
	int penalty;

public:
	Room(int capacity, int penalty, int id);
	int getCapacity() const;
	int getPenalty() const;
	int getId() const;
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

struct dateComparator {
	bool operator()(const std::string& a, const std::string& b) const {
		Date dA = Date(a);
		Date dB = Date(b);
		return dA < dB;
	}
};
