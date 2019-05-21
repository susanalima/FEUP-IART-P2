#include "DataExam.h"

#include <iostream>
#include <fstream>  
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>  


DataExam::DataExam()
{
	this->readExamInfo();

	std::cout << this->duration.first << std::endl;
	std::cout << this->duration.second << std::endl;

	std::cout << this->nrStudents.first << std::endl;
	std::cout << this->nrStudents.second << std::endl;


	this->buildExams();
}

DataExam::DataExam(int nrExams, std::pair<int, int> nrStudents, std::pair<int, int> duration)
{
	this->nrExams = nrExams;
	this->nrStudents = nrStudents;
	this->duration = duration;
	this->buildExams();
}

void DataExam::readExamInfo()
{
	std::string line;
	std::ifstream input("infoExam.txt");
	if (input.good())
	{
		if (getline(input, line)) {
			this->nrExams = std::stoi(line);
		}

		int gCnt = 0;

		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

		
			int minDuration, maxDuration, minNrStudents, maxNrStudents, cnt = 0;
			while (getline(lineInput, cell, ','))
			{
				switch (cnt)
				{
				case 0:
					minNrStudents = stoi(cell);
					break;
				case 1:
					maxNrStudents = stoi(cell);
					break;
				case 2:
					minDuration = stoi(cell);
					break;
				case 3:
					maxDuration = stoi(cell);
					break;
				default:
					break;
				}

				cnt++;
			}

			this->nrStudents = std::pair<int, int>(minNrStudents, maxNrStudents);
			this->duration = std::pair<int, int>(minDuration, maxDuration);

			gCnt++;
			if (gCnt == 1)
				break;

		}
		input.close();
	}
}

void DataExam::writeExams()
{
	std::vector<int> students;
	std::ofstream outfile("exams.txt");
	outfile << "Exams:" << this->nrExams << std::endl;
	for (int i = 0; i < this->nrExams; i++) {
		outfile << this->exams.at(i).duration << ", ";
		students = this->exams.at(i).students;
		for (unsigned int j = 0; j < students.size(); j++) {
			outfile << students.at(j) << ", ";
		}
		outfile << std::endl;
	}
	outfile.close();
}

void DataExam::buildExams()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int eDuration, eNrStudents;
	int lastStudent = 0;
	int j, index;
	for (int i = 0; i < this->nrExams; i++) {
		eDuration = mt() % (this->duration.second - this->duration.first + 1) + this->duration.first;
		eNrStudents = mt() % (this->nrStudents.second - this->nrStudents.first + 1) + this->nrStudents.first;
		
		std::vector<int> eStudents;
		j = 0;
		for (; j < eNrStudents; j++) {
			int n = mt() % 100 + 1;
			if (n <= 20)
				index = mt() % this->nrExams * this->nrStudents.second + 1;
			else
				index = j + lastStudent;
			eStudents.push_back(index);
		}
		lastStudent += j;
		this->exams.push_back(Exam(eDuration, eStudents));
	}
}
