
#include "Exam.h"
#include <ctime>
#include <sstream>
#include <random>

Exam::Exam(int duration, std::vector<int> students) {
	this->duration = duration;
	this->students = students;
	std::sort(this->students.begin(), this->students.end());
}

int Exam::getDuration() 
{
	return this->duration;
}

std::vector<int> Exam::getStudents() 
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

