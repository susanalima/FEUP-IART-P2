#include "Genetics.h"

#include <random>
#include <iostream>
#include <set>

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

//mudar penalidades mais pesadas para serem do estilo constante*nrExames ou algo assim
int Genetics::evaluateSolution(Node* solution)
{
	int penalty = 0; //the solution penalty / cost
	std::vector<std::pair<int, int>> schedule = solution->getAnswers();
	std::set<int> countPeriods;
	std::vector<Exam> exams = this->data->getExams();
	std::vector<Period> periods = this->data->getPeriods();
	std::vector<Room> rooms = this->data->getRooms();
	int roomIndex, periodIndex;

	for (int i = 0; i < schedule.size(); i++) {

		Exam exam = exams.at(i);

		periodIndex = schedule.at(i).first;
		Period period = periods.at(periodIndex);

		roomIndex = schedule.at(i).second;
		Room room = rooms.at(roomIndex);

		auto value = countPeriods.insert(periodIndex);
		if (!value.second)  // if the period is already being used
			penalty += 2000;  //penalty of dupliated period

		penalty += room.getPenalty(); //penalty of using the room
		penalty += period.getPenalty(); //penalty of using the period

		if (exam.getStudentsCnt() > room.getCapacity())
			penalty += 1000;  //penalty of exceding the room's capacity
		if (exam.getDuration() > period.getDuration())
			penalty += 1000;  //penalty of exceding the period's duration

		/*std::cout << periodIndex << ":" << roomIndex << "->" << exam.getDuration() << ":" << exam.getStudentsCnt() <<
			"->" << period.getDuration() << ":" << period.getPenalty() << "->" << room.getCapacity() << ":" << room.getPenalty() <<
			"->" << penalty << std::endl << std::endl;*/
	
	}
	//verificar se um periodo so e usado 1 vez (done)
	//verificar se a capacidade das salas nao e excedida (done)
	//verificar se a duraçao do exame nao excede a duraçao do periodo (done)
	//verificar overlaps de estudantes
	//etc
	return penalty;
}


//o best vai ser o que tiver menor penalty -> implica que so vai parar ao fim de maxNoGenerations, nunca sabe se ha ou nao um melhor
Node Genetics::solve(std::unordered_set<Node> population)
{
	for (int generationsCount = 0; generationsCount < this->maxNoGenerations; generationsCount++) {

		std::unordered_set<Node> newPopulation;
		insertPopulationBestElements(&population, &newPopulation);

		int currentPopulationSize = 0;
		Node elem1, elem2, child;
		int penalty;

		while (currentPopulationSize < this->populationSize) {


			elem1 = this->randomSelection(&newPopulation);
			elem2 = this->randomSelection(&newPopulation);

			child = reproduce(&elem1, &elem2);

			mutate(&child);

			penalty = evaluateSolution(&child);

			auto value = newPopulation.insert(child);

			if (value.second)  // if the value was succesfully inserted
				currentPopulationSize++;
		}
	}

	return this->best;
}


