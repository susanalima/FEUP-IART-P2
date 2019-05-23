#include "SimulatedAnnealing.h"

#include <math.h>
#include <random>
#include <functional>
#include <chrono>
#include <vector>

SimulatedAnnealing::SimulatedAnnealing()
{
}

SimulatedAnnealing::SimulatedAnnealing(Data* data, Node initial, double temperature, double coolingRate)
{
	this->data = data;
	this->initial = initial;
	this->best = initial;
	this->temperature = temperature;
	this->minTemperature = 0.001;
	this->coolingRate = coolingRate;

	this->data->evaluateSolution(&(this->best));
}

Node SimulatedAnnealing::solve()
{
	while (true) {
		if (this->temperature < this->minTemperature) {
			return this->best;
		}
		Node neighbor = findRandomNeighbor(this->best);
		int variation = this->best.getValue() - neighbor.getValue();
		if (variation > 0) {
			this->best = neighbor;
		}
		else {
			double pAcceptance = exp(variation / this->temperature);
			std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			auto real_rand = std::bind(std::uniform_real_distribution<double>(0, 1), std::mt19937(seed));
			if (pAcceptance > real_rand()) {
				this->best = neighbor;
			}
		}
		this->temperature *= this->coolingRate;
	}
}

void SimulatedAnnealing::printBest()
{
	this->best.print();
}

Node SimulatedAnnealing::findExamRandomNeighbor(Node current, int index)
{
	std::pair<int, int> curr = current.getAnswer(index);
	std::vector<Node> storage;

	Node tmp;
	//[-X, Y]
	if (curr.first > 0) {
		tmp = current;
		tmp.setAnswer(index, curr.first - 1, curr.second);
		this->data->evaluateSolution(&tmp);
		storage.push_back(tmp);
	}
	//[+X, Y]
	if (data->getPeriods().size() > (unsigned int)curr.first + 1) {
		tmp = current;
		tmp.setAnswer(index, curr.first + 1, curr.second);
		this->data->evaluateSolution(&tmp);
		storage.push_back(tmp);
	}
	//[X, -Y]
	if (curr.second > 0) {
		tmp = current;
		tmp.setAnswer(index, curr.first, curr.second - 1);
		this->data->evaluateSolution(&tmp);
		storage.push_back(tmp);
	}
	//[X, +Y]
	if (data->getRooms().size() > (unsigned int)curr.second + 1) {
		tmp = current;
		tmp.setAnswer(index, curr.first, curr.second + 1);
		this->data->evaluateSolution(&tmp);
		storage.push_back(tmp);
	}

	std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	auto index_rand = std::bind(std::uniform_int_distribution<int>(0, storage.size() - 1), std::mt19937(seed));

	return storage.at(index_rand());
}

Node SimulatedAnnealing::findRandomNeighbor(Node current)
{
	std::vector<Node> storage;
	for (size_t i = 0; i < current.getAnswersSize(); i++)
	{
		Node tmp = findExamRandomNeighbor(current, i);
		storage.push_back(tmp);
	}
	std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	auto index_rand = std::bind(std::uniform_int_distribution<int>(0, storage.size() - 1), std::mt19937(seed));

	return storage.at(index_rand());
}
