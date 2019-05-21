#include "DataPeriod.h"
#include <iostream>
#include <iomanip>

DataPeriod::DataPeriod()
{
	this->readPeriodInfo();
	this->buildPeriods();
}

DataPeriod::DataPeriod(int nrPeriods, std::pair<int, int> day, std::pair<int, int> month, int year, std::pair<int, int> hour, std::pair<int, int> duration, std::pair<int, int> penalty)
{
	this->nrPeriods = nrPeriods;
	this->day = day;
	this->month = month;
	this->year = year;
	this->hour = hour;
	this->duration = duration;
	this->penalty = penalty;
	
	buildPeriods();
}

void DataPeriod::readPeriodInfo()
{
	std::string line;
	std::ifstream input("infoPeriod.txt");
	if (input.good())
	{
		if (getline(input, line)) {
			this->nrPeriods = std::stoi(line);
		}

		int gCnt = 0;

		int t0, t1, t2, t3, t4;
		while (getline(input, line))
		{
			std::stringstream lineInput(line);
			std::string cell;

			int  cnt = 0;
			while (getline(lineInput, cell, ','))
			{
				switch (cnt)
				{
				case 0:
					t0 = stoi(cell);
					break;
				case 1:
					t1 = stoi(cell);
					break;
				case 2:
					t2 = stoi(cell);
					break;
				case 3:
					t3 = stoi(cell);
					break;
				case 4:
					t4 = stoi(cell);
					break;
				default:
					break;
				}

				cnt++;
			}

			switch (gCnt)
			{
			case 0:
				this->day = std::pair<int, int>(t0, t1);
				this->month = std::pair<int, int>(t2, t3);
				this->year = t4;
				break;
			case 1:
				this->hour = std::pair<int, int>(t0, t1);
				break;
			case 2:
				this->duration = std::pair<int, int>(t0, t1);
				break;
			case 3:
				this->penalty = std::pair<int, int>(t0, t1);
				break;
			default:
				break;
			}

			gCnt++;
			if (gCnt == 5)
				break;

		}
		input.close();
	}

}

void DataPeriod::writePeriods()
{
	std::ofstream outfile("periods.txt");
	outfile << "Periods:" << this->nrPeriods << std::endl;
	for (int i = 0; i < this->nrPeriods; i++) {
		Period period = this->periods.at(i);
		outfile << std::setw(2) << std::setfill('0') << period.day << ":";
		outfile << std::setw(2) << std::setfill('0') << period.month << ":";
		outfile <<  period.year << ", ";
		outfile << std::setw(2) << std::setfill('0') << period.hour << ":00:00, ";
		outfile << period.duration << ", " << period.penalty << std::endl;
	}
	outfile.close();
}

void DataPeriod::buildPeriods()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int pDay, pMonth, pYear, pHour, pDuration, pPenalty;
	for (int i = 0; i < this->nrPeriods; i++) {
		pYear = this->year;
		pDay = mt() % (this->day.second - this->day.first + 1) + this->day.first;
		pMonth = mt() % (this->month.second - this->month.first + 1) + this->month.first;
		pHour = mt() % (this->hour.second - this->hour.first + 1) + this->hour.first;
		pDuration = mt() % (this->duration.second - this->duration.first + 1) + this->duration.first;
		pPenalty = mt() % (this->penalty.second - this->penalty.first + 1) + this->penalty.first;
		this->periods.push_back(Period(pDay, pMonth, pYear, pHour, pDuration, pPenalty));
	}

}
