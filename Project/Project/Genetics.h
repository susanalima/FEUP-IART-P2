#pragma once

#include "Data.h"
#include <set>

class Genetics {

	Data* data;
	Node initial;
	Node best;
	int populationSize;   //maximum number of individuals generated for generation
	int maxNoGenerations;  // maximum number of generations
	int mutationProbability;  // mutation probability (percentage 0-100)
	int parentEliteNo;    //nr of elements received from the previous generation (the better eliteNo)
	long maxTimeMilliseconds; //the maximum time in milliseconds that the algorithm is to run for

public:

	Genetics();
	Genetics(Data* data, Node initial);
	Genetics(Data* data, Node initial, int populationSize, int maxNoGenerations, int mutationProbability, int parentEliteNo, long maxTimeMilliseconds);
	
	Node solve(std::set<Node> population);

	void printBest();

private:

	Node randomSelection(std::set<Node>* population);
	void mutate(Node* elem );
	Node reproduce(Node* elem1, Node* elem2);
	void insertPopulationBestElements(std::set<Node>* prevPopulation, std::set<Node>* newPopulation);
	int random(int min, int max);
};