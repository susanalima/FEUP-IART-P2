#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

#include "HillClimbing.h"
#include "SimulatedAnnealing.h"
#include "Genetics.h"

using namespace std;

void mainMenu(Data data);

int main()
{
	string fileLoc;
	cout << "Enter data files location:" << endl;
	getline(cin, fileLoc);

	Data data = Data(fileLoc);

	mainMenu(data);

}

void executeMultiple(Data data) {
	std::vector<Node> initials;

	Node best;
	HillClimbing hc;
	SimulatedAnnealing sa;
	Genetics gn;
	set<Node> population;

	double temp = 100;
	double coolRate = 0.8;

	auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;

	std::ofstream out("out_all.txt");
	std::streambuf* coutbuf = std::cout.rdbuf(); 
	std::cout.rdbuf(out.rdbuf());

	//5 diff initial states
	cout << "Initial States:" << endl;
	for (size_t j = 0; j < 5; j++)
	{
		Node ini = data.generateInitialState();
		data.evaluateSolution(&ini);
		initials.push_back(ini);
		cout << "ID: " << j + 1 << endl;
		data.printNodeInfo(&ini);
	}

	//run the 3 algorithms
	for (size_t i = 0; i < 3; i++)
	{
		//diff initial states
		for (size_t j = 0; j < initials.size(); j++)
		{
			Node ini = initials.at(j);
			double count = 0;
			//5x per ini state
			for (size_t k = 0; k < 5; k++)
			{
				cout << i << ":" << j << ":";
				switch (i)
				{
				case 0:
					hc = HillClimbing(&data, ini);
					start = std::chrono::high_resolution_clock::now();
					best = hc.solve();
					finish = std::chrono::high_resolution_clock::now();
					elapsed = finish - start;
					count += elapsed.count();
					break;
				case 1:
					sa = SimulatedAnnealing(&data, ini, temp, coolRate);
					start = std::chrono::high_resolution_clock::now();
					best = sa.solve();
					finish = std::chrono::high_resolution_clock::now();
					elapsed = finish - start;
					count += elapsed.count();
					break;
				case 2:
					gn = Genetics(&data, ini, 50, 100000000000, 30, 15, 5000);
					population.clear();
					population.insert(ini);
					start = std::chrono::high_resolution_clock::now();
					best = gn.solve(population);
					finish = std::chrono::high_resolution_clock::now();
					elapsed = finish - start;
					count += elapsed.count();
					break;
				}
				cout << elapsed.count() << ":" << best.getNoFaults() << ":" << best.getPenalty() << endl;
			}
			cout << "\tMean:\t" << count / 5 << endl;
		}
	}

	std::cout.rdbuf(coutbuf);

	cout << "Done!" << endl;
}

void executeSADiff(Data data) {

	Node best;
	SimulatedAnnealing sa;
	std::vector<Node> initials;

	double temp = 100;

	auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;

	std::ofstream out("out_cooling.txt");
	std::streambuf* coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	//5 diff initial states
	cout << "Initial States:" << endl;
	for (size_t j = 0; j < 5; j++)
	{
		Node ini = data.generateInitialState();
		data.evaluateSolution(&ini);
		initials.push_back(ini);
		cout << "ID: " << j + 1 << endl;
		data.printNodeInfo(&ini);
	}

	for (size_t i = 0; i < initials.size(); i++)
	{
		Node ini = initials.at(i);
		for (size_t j = 1; j < 10; j++)
		{
			double coolingRate = 0.1 * j;
			double count = 0;
			//10x per ini state
			for (size_t k = 0; k < 10; k++)
			{
				cout << i << ":" << coolingRate << ":" << k << ":";
				sa = SimulatedAnnealing(&data, ini, temp, coolingRate);
				start = std::chrono::high_resolution_clock::now();
				best = sa.solve();
				finish = std::chrono::high_resolution_clock::now();
				elapsed = finish - start;
				count += elapsed.count();
				cout << elapsed.count() << ":" << best.getNoFaults() << ":" << best.getPenalty() << endl;
			}
			cout << "\tMean:\t" << count / 10 << endl;
		}
	}
	std::cout.rdbuf(coutbuf);
	cout << "Done!" << endl;
}

void executeGenDiff(Data data) {

	Node best;
	Genetics gn;
	set<Node> population;
	std::vector<Node> initials;

	double temp = 100;

	auto start = std::chrono::high_resolution_clock::now();
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;

	std::ofstream out("out_gens.txt");
	std::streambuf* coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	//10 diff initial states
	cout << "Initial States:" << endl;
	for (size_t j = 0; j < 5; j++)
	{
		Node ini = data.generateInitialState();
		data.evaluateSolution(&ini);
		initials.push_back(ini);
		cout << "ID: " << j + 1 << endl;
		data.printNodeInfo(&ini);
	}

	for (size_t i = 0; i < initials.size(); i++)
	{
		Node ini = initials.at(i);
		for (size_t j = 1; j < 10; j++)
		{
			double mutProb = 10 * j;
			double count = 0;
			//10x per ini state
			for (size_t k = 0; k < 10; k++)
			{
				cout << i << ":" << mutProb << ":" << k << ":";
				gn = Genetics(&data, ini, mutProb);
				population.clear();
				population.insert(ini);
				start = std::chrono::high_resolution_clock::now();
				best = gn.solve(population);
				finish = std::chrono::high_resolution_clock::now();
				elapsed = finish - start;
				count += elapsed.count();
				cout << elapsed.count() << ":" << best.getNoFaults() << ":" << best.getPenalty() << endl;
			}
			cout << "\tMean:\t" << count / 10 << endl;
		}
	}
	std::cout.rdbuf(coutbuf);
	cout << "Done!" << endl;
}

void mainMenuDisplay() {
	cout << "Options:" << endl;
	cout << "\t1.\tExecute Hill Climbing once." << endl;
	cout << "\t2.\tExecute Simulated Annealing once." << endl;
	cout << "\t3.\tExecute Genetics Algorithm once." << endl;
	cout << "\t4.\tExecute Multiple." << endl;
	cout << "\t5.\tExecute Simulated Annealing at different cooling rates." << endl;
	cout << "\t6.\tExecute Genetics at different mutation rates." << endl;
	cout << "\t7.\tExit." << endl;
}

void mainMenu(Data data) {
	int opt;

	Node ini = data.generateInitialState();
	data.evaluateSolution(&ini);

	Node best;
	HillClimbing hc;
	SimulatedAnnealing sa;
	Genetics gn;
	set<Node> population;

	double temp = 0;
	double coolRate = 0;
	int mutProbability = 0;
	int pop = 0;

	char option;

	bool ending = false;
	while (!ending)
	{
		population.clear();
		mainMenuDisplay();
		cin >> opt;

		switch (opt)
		{
		case 1:
			cout << "Initial State Info" << endl;
			data.printNodeInfo(&ini);
			cout << "HillClimbing Info" << endl;
			hc = HillClimbing(&data, ini);
			best = hc.solve();
			data.printNodeInfo(&best);
			data.buildDisplay(&best, "Hill Climbing");
			break;
		case 2:
			cout << "Initial State Info" << endl;
			data.printNodeInfo(&ini);
			cout << "SimulatedAnnealing Info" << endl;
			cout << "\tInitial Temperature: " << endl;
			cin >> temp;
			cout << "\tCool rate: " << endl;
			cin >> coolRate;

			sa = SimulatedAnnealing(&data, ini, temp, coolRate);
			best = sa.solve();
			data.printNodeInfo(&best);
			data.buildDisplay(&best, "Simulated Annealing");
			break;
		case 3:
			cout << "Initial State Info" << endl;
			data.printNodeInfo(&ini);
			cout << "Genetics Info" << endl;
			cout << "\tMutation Probability (0-100): " << endl;
			cin >> mutProbability;

			cout << "\tTime based?(y/n) " << endl;
			cin >> option;

			cout << "\tEnter population size: " << endl;
			cin >> pop;

			if (opt == 'y' || opt == 'Y') {
				cout << "\tEnter max time(ms): " << endl;
				cin >> temp;
				gn = Genetics(&data, ini, pop, 100000000000, mutProbability, 15, temp);
			}
			else {
				cout << "\tEnter max iterations: " << endl;
				cin >> temp;
				gn = Genetics(&data, ini, pop, temp, mutProbability, 15, 100000000000);
			}

			gn = Genetics(&data, ini, mutProbability);
			population.insert(ini);
			best = gn.solve(population);
			data.printNodeInfo(&best);
			data.buildDisplay(&best, "Genetics");
			break;
		case 4:
			executeMultiple(data);
			break;
		case 5:
			executeSADiff(data);
			break;
		case 6:
			executeGenDiff(data);
			break;
		default:
			ending = true;
			break;
		}
	}

	
}