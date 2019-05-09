#include <iostream>

#include "HillClimbing.h"
#include "Genetics.h"

using namespace std;

int main()
{
	Data data = Data();
	Node ini = data.generateInitialState();
	ini.print();
	
	cout << endl << endl;

	HillClimbing hc(&data, ini);
	Node best = hc.solve();
	best.print();

	cout << endl << endl;

	Genetics gn(&data, ini);
	set<Node> population;
	population.insert(ini);
	Node gnbest = gn.solve(population);
	gnbest.print();
	std::cout << gnbest.getNoFaults() << std::endl;
	std::cout << gnbest.getPenalty() << std::endl;



}
