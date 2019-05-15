#include "HillClimbing.h"

Node HillClimbing::solve()
{
	Node previous = this->best;
	while (true) {
		Node neighbor = findBestNeighbor(this->best);
		if (neighbor <= this->best && neighbor != previous) {
			previous = this->best;
			this->best = neighbor;
		} 
		else
			return this->best;
	}
	return this->best;
}

void HillClimbing::printBest()
{
	this->best.print();
}

HillClimbing::HillClimbing(Data *data, Node initial)
{
	this->data = data;
	this->initial = initial;
	this->best = initial;

	this->data->evaluateSolution(&(this->best));
}

Node HillClimbing::findExamBestNeighbor(Node current, int index)
{
	Node best = current;
	std::pair<int, int> curr = current.getAnswer(index);

	Node tmp;
	//[-X, Y]
	if (curr.first > 0) {
		tmp = current;
		tmp.setAnswer(index, curr.first - 1, curr.second);
		this->data->evaluateSolution(&tmp);
		if (tmp <= best)
			best = tmp;
	}
	//[+X, Y]
	if (data->getPeriods().size() > (unsigned int) curr.first + 1) {
		tmp = current;
		tmp.setAnswer(index, curr.first + 1, curr.second);
		this->data->evaluateSolution(&tmp);
		if (tmp <= best)
			best = tmp;
	}
	//[X, -Y]
	if (curr.second > 0) {
		tmp = current;
		tmp.setAnswer(index, curr.first, curr.second - 1);
		this->data->evaluateSolution(&tmp);
		if (tmp <= best)
			best = tmp;
	}
	//[X, +Y]
	if (data->getRooms().size() > (unsigned int) curr.second + 1) {
		tmp = current;
		tmp.setAnswer(index, curr.first, curr.second + 1);
		this->data->evaluateSolution(&tmp);
		if (tmp <= best)
			best = tmp;
	}

	return best;
}

Node HillClimbing::findBestNeighbor(Node current)
{
	Node node = current;
	for (size_t i = 0; i < current.getAnswersSize(); i++)
	{
		Node tmp = findExamBestNeighbor(current, i);
		if (node < tmp)
			node = tmp;
	}
	return node;
}
