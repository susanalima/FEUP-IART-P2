#include "Utils.h"

#include <ctime>
#include <sstream>
#include <random>


Exam::Exam(int duration, std::vector<int> students)
{
	this->duration = duration;
	this->students = students;
	std::sort(this->students.begin(), this->students.end());
}

int Exam::getDuration() const
{
	return this->duration;
}

int Exam::getStudentsCnt() const
{
	return this->students.size();
}

std::vector<int> Exam::getStudents() const
{
	return this->students;
}

std::vector<int> Exam::getOverlappingStudents(Exam* exam)
{
	std::vector<int> examStudents = exam->getStudents();
	std::vector<int> overlappingStudents;

	auto it = std::set_intersection(examStudents.begin(), examStudents.end(), this->students.begin(), this->students.end(), back_inserter(overlappingStudents));

	return overlappingStudents;
}

void Exam::addIncompatibleExam(int examIndex)
{
	this->incompatibleExams.push_back(examIndex);
}

std::vector<int> Exam::getIncompatibleExams() const
{
	return this->incompatibleExams;
}

void Exam::addPeriodConstraint(int examIndex, std::string constraint)
{
	this->periodConstrains.push_back(std::pair<int, std::string>(examIndex, constraint));
}

std::vector<std::pair<int, std::string>> Exam::getPeriodConstraints() const
{
	return this->periodConstrains;
}

Period::Period(Date date, Time time, int duration, int penalty, int id)
{
	this->date = date;
	this->time = time;
	this->duration = duration;
	this->penalty = penalty;
	this->id = id;
	this->endTime = Time(0, duration, 0) + this->time;
}

Date Period::getDate() const
{
	return this->date;
}

Time Period::getTime() const
{
	return this->time;
}

int Period::getDuration() const
{
	return this->duration;
}

int Period::getPenalty() const
{
	return this->penalty;
}

int Period::getId() const
{
	return this->id;
}

Time Period::getEndTime() const
{
	return this->endTime;
}

Room::Room(int capacity, int penalty, int id)
{
	this->capacity = capacity;
	this->penalty = penalty;
	this->id = id;
}

int Room::getCapacity() const
{
	return this->capacity;
}

int Room::getPenalty() const
{
	return this->penalty;
}

int Room::getId() const
{
	return this->id;
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
	this->time_ = time;
	this->hours = 0;
	this->minutes = 0;
	this->seconds = 0;
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
	
	std::string t = "";
	if (hours < 10)
		t += "0";
	t += std::to_string(hours) + ":";
	if (minutes < 10)
		t += "0";
	t += std::to_string(minutes) + ":";
	if (seconds < 10)
		t += "0";
	t += std::to_string(seconds);
	this->time_ = t;
}

int Time::getHours() const
{
	return this->hours;
}

int Time::getMinutes() const
{
	return this->minutes;
}

int Time::getSeconds() const
{
	return this->seconds;
}

std::string Time::getTime() const
{
	return this->time_;
}

Date::Date()
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	this->day = ltm.tm_mday;
	this->month = ltm.tm_mon;
	this->year = ltm.tm_year;
	this->date = std::to_string(this->day) + ":" + std::to_string(this->month) + ":" + std::to_string(this->year);
}

Date::Date(std::string date)
{
	this->date = date;
	this->day = 0;
	this->month = 0;
	this->year = 0;
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

int Date::getDay() const
{
	return this->day;
}

int Date::getMonth() const
{
	return this->month;
}

int Date::getYear() const
{
	return this->year;
}

std::string Date::getDate() const
{
	return this->date;
}


FrontLoad::FrontLoad() : FrontLoad(0,0,0)
{
}

FrontLoad::FrontLoad(int nrExams, int nrPeriods, int penalty)
{
	this->nrExams = nrExams;
	this->nrPeriods = nrPeriods;
	this->penalty = penalty;
}

int FrontLoad::getNrExams() const
{
	return this->nrExams;
}

int FrontLoad::getNrPeriods() const
{
	return this->nrPeriods;
}

int FrontLoad::getPenalty() const
{
	return this->penalty;
}

InstitutionalWeightings::InstitutionalWeightings() 
{
	this->twoInRow = 0;
	this->twoInDay = 0;
	this->periodSpreed = 0;
	this->nonMixedDurations = 0;
	FrontLoad frontload();
	this->frontLoad = frontLoad;
}


InstitutionalWeightings::InstitutionalWeightings(int twoInRow, int twoInDay, int periodSpreed, int nonMixedDurations, int nrExams, int nrPeriods, int penalty)
{
	this->twoInRow = twoInRow;
	this->twoInDay = twoInDay;
	this->periodSpreed = periodSpreed;
	this->nonMixedDurations = nonMixedDurations;
	this->frontLoad = FrontLoad(nrExams, nrPeriods, penalty);
}

int InstitutionalWeightings::getTwoInRow() const
{
	return this->twoInRow;
}

int InstitutionalWeightings::getTwoInDay() const
{
	return this->twoInDay;
}

int InstitutionalWeightings::getPeriodSpreed() const
{
	return this->periodSpreed;
}

int InstitutionalWeightings::getNonMixedDurations() const
{
	return this->nonMixedDurations;
}

FrontLoad InstitutionalWeightings::getFrontLoad() const
{
	return this->frontLoad;
}



bool operator<(const Date& lhs, const Date& rhs)
{
	if (lhs.getYear() < rhs.getYear())
		return true;
	else if (lhs.getYear() == rhs.getYear()) {
		if (lhs.getMonth() < rhs.getMonth())
			return true;
		else if (lhs.getMonth() == rhs.getMonth()) {
			if (lhs.getDay() < rhs.getDay())
				return true;
		}
	}
	return false;
}

bool operator>(const Date& lhs, const Date& rhs)
{
	return rhs < lhs;
}

bool operator<=(const Date& lhs, const Date& rhs)
{
	return !(lhs > rhs);
}

bool operator>=(const Date& lhs, const Date& rhs)
{
	return !(lhs < rhs);
}

bool operator==(const Date& lhs, const Date& rhs)
{
	return lhs.getYear() == rhs.getYear() && lhs.getMonth() == rhs.getMonth() && lhs.getDay() == rhs.getDay();
}

bool operator<(const Time& lhs, const Time& rhs)
{
	if (lhs.getHours() < rhs.getHours())
		return true;
	else if (lhs.getHours() == rhs.getHours()) {
		if (lhs.getMinutes() < rhs.getMinutes())
			return true;
		else if (lhs.getMinutes() == rhs.getMinutes()) {
			if (lhs.getSeconds() < rhs.getSeconds())
				return true;
		}
	}
	return false;
}

bool operator>(const Time& lhs, const Time& rhs)
{
	return rhs < lhs;
}

bool operator<=(const Time& lhs, const Time& rhs)
{
	return !(lhs > rhs);
}

bool operator>=(const Time& lhs, const Time& rhs)
{
	return !(lhs < rhs);
}

bool operator==(const Time& lhs, const Time& rhs)
{
	return lhs.getHours() == rhs.getHours() && lhs.getMinutes() == rhs.getMinutes() && lhs.getSeconds() == rhs.getSeconds();
}



Time operator+(const Time& a, const Time& b) {
	int h, m, s;
	s = a.getSeconds() + b.getSeconds();
	m = a.getMinutes() + b.getMinutes() + s / 60;
	h = a.getHours() + b.getHours() + m / 60;
	m = m % 60;
	s = s % 60;

	Time result = Time(h, m, s);
	return result;
}


bool operator<(const Period& lhs, const Period& rhs)
{
	if (lhs.getDate() < rhs.getDate())
		return true;
	if (lhs.getDate() == rhs.getDate()) {
		if (lhs.getTime() < rhs.getTime())
			return true;
		else if (lhs.getTime() == rhs.getTime()) {
			if (lhs.getDuration() < rhs.getDuration())
				return true;
			else if (lhs.getDuration() == rhs.getDuration()) {
				return lhs.getPenalty() < rhs.getPenalty();
			}
		}
	}
	return false;
}

bool operator>(const Period& lhs, const Period& rhs)
{
	return rhs < lhs;
}

bool operator<=(const Period& lhs, const Period& rhs)
{
	return !(lhs > rhs);
}

bool operator>=(const Period& lhs, const Period& rhs)
{
	return !(lhs < rhs);
}

bool operator==(const Period& lhs, const Period& rhs)
{
	return lhs.getDate() == rhs.getDate() && lhs.getDuration() == rhs.getDuration() && lhs.getPenalty() == rhs.getPenalty() && lhs.getTime() == rhs.getTime();
}

bool operator<(const Room& lhs, const Room& rhs)
{
	if (lhs.getPenalty() < rhs.getPenalty())
		return true;
	else if (lhs.getPenalty() == rhs.getPenalty()) {
		if (lhs.getCapacity() < rhs.getCapacity())
			return true;
	}
	return false;
}

bool operator>(const Room& lhs, const Room& rhs)
{
	return rhs < lhs;
}

bool operator<=(const Room& lhs, const Room& rhs)
{
	return !(lhs > rhs);
}

bool operator>=(const Room& lhs, const Room& rhs)
{
	return !(lhs < rhs);
}

bool operator==(const Room& lhs, const Room& rhs)
{
	return lhs.getCapacity() == rhs.getCapacity() && lhs.getPenalty() == rhs.getPenalty();
}

bool operator<(const Exam& lhs, const Exam& rhs)
{
	return lhs.getStudentsCnt() < rhs.getStudentsCnt();
}

bool operator==(const Exam& lhs, const Exam& rhs)
{
	return lhs.getStudentsCnt() == rhs.getStudentsCnt();
}
