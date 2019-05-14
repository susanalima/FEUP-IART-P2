#include "Genetics.h"

#include <random>
#include <iostream>
#include <map>
#include <set>
#include <iterator>     

Genetics::Genetics(Data* data, Node initial) : Genetics(data, initial, 50, 1000, 30, 15)
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

//change to choose from the best
Node Genetics::randomSelection(std::set<Node>* population)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int n = mt() % population->size();
	int index = 0;
	auto it = population->begin();
	std::advance(it, n);
	return (*it);
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


//o best vai ser o que tiver menor penalty 
Node Genetics::solve(std::set<Node> population)
{
	int bestPenalty = -1, bestNoFaults, penalty, noFaults;
	int maxRoomPeriodPenalty = this->data->getMaxRoomPenalty() + this->data->getMaxPeriodPenalty();
	Node elem1, elem2, child;

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

			//todo este or e estupido, arranjar outra condiçao de paragem
			if (noFaults == 0 && penalty == 0) /*
				|| (this->data->getExamsCnt() == this->data->getPeriodsCnt() && this->data->getExamsCnt() == this->data->getRoomsCnt()
					&& penalty == maxRoomPeriodPenalty)))*/ {
				
				return this->best;
			}
		
			newPopulation.insert(child);
		}
		population = newPopulation;
	}
	return this->best;
}


