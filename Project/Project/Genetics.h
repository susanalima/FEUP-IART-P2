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

public:

	Genetics(Data* data, Node initial);
	Genetics(Data* data, Node initial, int populationSize, int maxNoGenerations, int mutationProbability, int parentEliteNo);
	
	Node solve(std::set<Node> population);

	void printBest();

private:

	Node randomSelection(std::set<Node>* population);
	void mutate(Node* elem );
	Node reproduce(Node* elem1, Node* elem2);
	void insertPopulationBestElements(std::set<Node>* prevPopulation, std::set<Node>* newPopulation);
	void evaluateSolution(Node* solution);
	int applyPeriodHardConstraints(int index, std::vector<Period>* periods, std::vector<Exam>* exams, std::vector<std::pair<int, int>>* schedule, Period* period, Exam* exam, int periodIndex);
	int applyGeneralHardConstraints(int index, std::map<std::pair<int, int>,int>* examSlot, std::vector<std::pair<int, int>>* schedule, Exam* exam, Period* period, Room* room);

};