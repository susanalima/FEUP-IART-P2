
#include "Exam.h"

Exam::Exam(int duration, std::vector<int> students) {
	this->duration = duration;
	this->students = students;
}

int Exam::getDuration() 
{
	return this->duration;
}

std::vector<int> Exam::getStudents() 
{
	return this->students;
}
