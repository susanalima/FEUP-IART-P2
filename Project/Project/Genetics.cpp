#include "Genetics.h"

#include <random>
#include <iostream>
#include <map>
#include <set>
#include <iterator>     
#include <chrono>

Genetics::Genetics(Data* data, Node initial) : Genetics(data, initial, 50, 1000, 30, 15, 0)
{
}

Genetics::Genetics(Data* data, Node initial, int populationSize, int maxNoGenerations, int mutationProbability, int parentEliteNo, long maxTimeMilliseconds)
{
	this->data = data;
	this->initial = initial;
	this->best = initial;
	this->populationSize = populationSize;
	this->maxNoGenerations = maxNoGenerations;
	this->mutationProbability = mutationProbability;
	this->parentEliteNo = parentEliteNo;
	this->maxTimeMilliseconds = maxTimeMilliseconds;
}


void Genetics::printBest()
{
	this->best.print();
}

//change to choose from the best
Node Genetics::randomSelection(std::set<Node>* population)
{
	int n = this->random(0, population->size() -1);
	int index = 0;
	auto it = population->begin();
	std::advance(it, n);
	return (*it);
}


void Genetics::mutate(Node* elem)
{
	int n = this->random(1,100);  //random number between 1 and 100

	if (n > this->mutationProbability)
		return;

	int nrOfMutations = this->random(1, elem->getAnswersSize());
	int index, randomPeriod, randomRoom;

	for (int i = 0; i < nrOfMutations; i++) {
		 index = this->random(0, elem->getAnswersSize() -1);
		 randomPeriod = this->random(0, this->data->getPeriodsCnt() -1);
		 randomRoom = this->random(0, this->data->getRoomsCnt() -1);
		 elem->setAnswer(index, randomPeriod, randomRoom);
	}
}

Node Genetics::reproduce(Node* elem1, Node* elem2)
{
	int c = this->random(0, elem1->getAnswersSize() - 1);

	std::vector<std::pair<int, int>> elemAnswers = elem1->getAnswers();
	std::vector<std::pair<int, int>> answers1(elemAnswers.begin(), elemAnswers.begin()+ c);
	elemAnswers = elem2->getAnswers();
	std::vector<std::pair<int, int>> answers2(elemAnswers.begin() + c, elemAnswers.begin() + elem2->getAnswersSize());
	answers1.insert(answers1.end(), answers2.begin(), answers2.end());

	Node child = Node();
	child.setAnswers(answers1);
	return child;
}

//permite elitismo, manter os parentEliteNo progenitores na populaçao
void Genetics::insertPopulationBestElements(std::set<Node>* prevPopulation, std::set<Node>* newPopulation)
{
	int ad = this->parentEliteNo;
	if (prevPopulation->size() < this->parentEliteNo)
		ad = prevPopulation->size();
	auto bg = prevPopulation->begin();
	std::advance(bg, ad);
	newPopulation->insert(prevPopulation->begin(), bg );
}

int Genetics::random(int min, int max)
{
	std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> gen(min, max); 
	int r = gen(rng);
	return r;
}


//o best vai ser o que tiver menor penalty 
Node Genetics::solve(std::set<Node> population)
{
	int bestPenalty = -1, bestNoFaults, penalty, noFaults;
	Node elem1, elem2, child;

	const auto begin = clock();

	for (int generationsCount = 0; generationsCount < this->maxNoGenerations; generationsCount++) {

		std::set<Node> newPopulation;
		insertPopulationBestElements(&population, &newPopulation);

		//todo refactor -> mudar para outra função
		for (int currentPopulationSize = 0; currentPopulationSize < this->populationSize; currentPopulationSize++) {

			elem1 = this->randomSelection(&population);
			elem2 = this->randomSelection(&population);
		
			child = reproduce(&elem1, &elem2);
			mutate(&child);

			this->data->evaluateSolution(&child);

			penalty = child.getPenalty();
			noFaults = child.getNoFaults();
			if (bestPenalty == -1 || (penalty <= bestPenalty && noFaults <= bestNoFaults) || noFaults < bestNoFaults) {
				this->best = child;
				bestPenalty = penalty;
				bestNoFaults = noFaults;
			}

			if ((noFaults == 0 && penalty == 0) 
				|| (maxTimeMilliseconds > 0L && ((clock() - begin) / CLOCKS_PER_SEC *1000  >= maxTimeMilliseconds))) {
				
				return this->best;
			}
		
			newPopulation.insert(child);
		}
		population = newPopulation;
	}
	return this->best;
}


