#pragma once

#include "Data.h"

class SimulatedAnnealing {

	Data* data;
	Node initial;
	Node best;
	double temperature;
	double minTemperature;
	double coolingRate; //temperature reduction rate

public:

	SimulatedAnnealing();
	SimulatedAnnealing(Data* data, Node initial, double temperature, double coolingRate);
	Node solve();

	void printBest();

private:

	Node findExamRandomNeighbor(Node current, int index);
	Node findRandomNeighbor(Node current);
};