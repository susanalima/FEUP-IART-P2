#include "Genetics.h"

#include <random>
#include <iostream>


Genetics::Genetics(Data* data, Node initial) : Genetics(data, initial, 50, 1000, 20, 2)
{
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
	int n = mt() % 100 + 1;  //random number between 1 and 100

	if (n > this->mutationProbability)
		return;

	int nrOfMutations = mt() % elem->getAnswersSize() + 1;
	int index, randomPeriod, randomRoom;

	for (int i = 0; i < nrOfMutations; i++) {
		 index = mt() % elem->getAnswersSize();
		 randomPeriod = mt() % this->data->getPeriodsCnt() ;
		 randomRoom = mt() % this->data->getRoomsCnt();
		 elem->setAnswer(index, randomPeriod, randomRoom);
	}
}

/*
	pair<period, room>
std::vector<std::pair<int, int>> answers; */
Node Genetics::reproduce(Node* elem1, Node* elem2)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int c = mt() % elem1->getAnswersSize() ;

	std::vector<std::pair<int, int>> elemAnswers = elem1->getAnswers();
	std::vector<std::pair<int, int>> answers1(elemAnswers.begin(), elemAnswers.begin()+ c);
	elemAnswers = elem2->getAnswers();
	std::vector<std::pair<int, int>> answers2(elemAnswers.begin() + c, elemAnswers.begin() + elem2->getAnswersSize());
	answers1.insert(answers1.end(), answers2.begin(), answers2.end());

	Node child = Node();
	child.setAnswers(answers1);
	return child;
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


