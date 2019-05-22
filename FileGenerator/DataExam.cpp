#include "DataExam.h"
#include "Utils.h"
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
	this->buildExams();
	this->buildIncompatibilitiesTable();
}

DataExam::DataExam(int nrExams, std::pair<int, int> nrStudents, std::pair<int, int> duration)
{
	this->nrExams = nrExams;
	this->nrStudents = nrStudents;
	this->duration = duration;
	this->buildExams();
	this->buildIncompatibilitiesTable();
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
			outfile << students.at(j);
			if (j < students.size() - 1)
				outfile << ", ";
		}
		outfile << std::endl;
	}
	outfile.close();
}

void DataExam::buildExams()
{
	
	int eDuration, eNrStudents;
	int lastStudent = 0;
	int j, index;
	for (int i = 0; i < this->nrExams; i++) {
		eDuration = random(this->duration.first, this->duration.second);
		eNrStudents = random(this->nrStudents.first, this->nrStudents.second);

		std::vector<int> eStudents;
		j = 0;
		for (; j < eNrStudents; j++) {
			int n = random(1, 100);
			if (n <= 10)
				index = random(1, this->nrExams * this->nrStudents.second);
			else
				index = j + lastStudent;
			eStudents.push_back(index);
		}
		lastStudent += j;
		this->exams.push_back(Exam(eDuration, eStudents));
	}
}

void DataExam::buildIncompatibilitiesTable()
{
	int nrOverlaps;
	for (int i = 0; i < this->exams.size(); i++) {
		Exam exam1 = this->exams.at(i);
		for (int j = i + 1; j < this->exams.size(); j++) {
			Exam exam2 = this->exams.at(j);
			nrOverlaps = exam1.getOverlappingStudents(&exam2).size();
			this->incompatibilitiesTable.insert(std::pair<std::pair<int, int>, int>(std::pair<int, int>(i, j), nrOverlaps));
		}
	}
}

void DataExam::writeIncompatibilitiesTable()
{
	std::vector<int> students;
	std::ofstream outfile("incompatibilitiesTable.txt");
	outfile << "incompatibilitiesTable:"  << std::endl;
	
	for (auto it = this->incompatibilitiesTable.begin(); it != this->incompatibilitiesTable.end(); it++)
	{
		outfile << it->first.first << ":" << it->first.second << "-" << it->second << std::endl;
	}
	outfile.close();
}
