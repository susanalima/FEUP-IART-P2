#include "Genetics.h"

#include <random>


Genetics::Genetics(Data* data, Node initial)
{
	Genetics(data, initial, 50, 1000, 20, 2);
}

Genetics::Genetics(Data* data, Node initial, int populationSize, int maxNoGenerations, int mutationProbability, int parentEliteNo)
{
	this->data = data;
	this->initial = initial;
	this->best = initial;
	this->populationSize = populationSize;
	this->maxNoGenerations = maxNoGenerations;
	this->mutationProbability = mutationProbability;
	this->parentEliteNo = parentEliteNo;
}


void Genetics::printBest()
{
	this->best.print();
}

Node Genetics::randomSelection(std::unordered_set<Node>* population)
{
	return Node();
}

void Genetics::mutate(Node* elem)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int n = mt() % 101;  //random number between 0 and 100

	if (n == 0)
		return;
	else if (n <= this->mutationProbability) {
		//mutate
	}

}

Node Genetics::reproduce(Node* elem1, Node* elem2)
{
	return Node();
}

void Genetics::insertPopulationBestElements(std::unordered_set<Node>* prevPopulation, std::unordered_set<Node>* newPopulation)
{

}


Node Genetics::solve(std::unordered_set<Node> population)
{
	//while()
	std::unordered_set<Node> newPopulation;

	insertPopulationBestElements(&population, &newPopulation);

	int currentPopulationSize = 0;
	Node elem1, elem2, child;
	while (currentPopulationSize < this->populationSize) {

		elem1 = this->randomSelection(&newPopulation);
		elem2 = this->randomSelection(&newPopulation);
		
		child = reproduce(&elem1, &elem2);

		mutate(&child);

		auto value = newPopulation.insert(child);

		if (value.second)  // if the value was succesfully inserted
			currentPopulationSize++;
	}

	return this->best;
}


