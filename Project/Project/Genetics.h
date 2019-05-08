#pragma once

#include "Data.h"
#include <unordered_set>

class Genetics {

	Data* data;
	Node initial;
	Node best;
	int populationSize;   //maximum number of individuals generated for generation
	int maxNoGenerations;  // maximum number of generations
	int mutationProbability;  // mutation probability (percentage 0-100)
	int parentEliteNo;    //nr of elements received from the previous generation (the better eliteNo)

public:

	Genetics(Data* data, Node initial);
	Genetics(Data* data, Node initial, int populationSize, int maxNoGenerations, int mutationProbability, int parentEliteNo);
	
	Node solve(std::unordered_set<Node> population);

	void printBest();

private:

	Node randomSelection(std::unordered_set<Node>* population);
	void mutate(Node* elem );
	Node reproduce(Node* elem1, Node* elem2);
	void insertPopulationBestElements(std::unordered_set<Node>* prevPopulation, std::unordered_set<Node>* newPopulation);
	int evaluateSolution(Node* solution);
};