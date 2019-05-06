#pragma once

#include "Data.h"

class HillClimbing {

	Data *data;
	Node initial;
	Node best;

public:

	HillClimbing(Data *data, Node initial);
	Node solve();

	void printBest();

private:

	Node findExamBestNeighbor(Node current, int index);
	Node findBestNeighbor(Node current);
};