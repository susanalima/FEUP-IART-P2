#include <iostream>

#include "HillClimbing.h"
#include "SimulatedAnnealing.h"
#include "Genetics.h"

using namespace std;

int main()
{

	cout << "Ini Info" << endl;

	Data data = Data();
	Node ini = data.generateInitialState();
	ini.print();
	
	cout << endl << endl;
	cout << "HillClimbing Info" << endl;
	cout << endl << endl;

	HillClimbing hc(&data, ini);
	Node bestHC = hc.solve();

	data.printNodeInfo(&bestHC);

	cout << endl << endl;
	cout << "SimulatedAnnealing Info" << endl;

	double temp = 1000;
	double coolRate = 0.70;

	cout << "Temperature: " << temp << endl;
	cout << "Cooling Rate: " << coolRate << endl;
	cout << endl << endl;
	
	SimulatedAnnealing sa(&data, ini, temp, coolRate);
	Node bestSA = sa.solve();

	data.printNodeInfo(&bestSA);

	cout << endl << endl;
	cout << "Genetics Info" << endl;
	cout << endl << endl;

	Genetics gn(&data, ini);
	set<Node> population;
	population.insert(ini);
	Node gnbest = gn.solve(population);
	/*gnbest.print();
	std::cout << gnbest.getNoFaults() << std::endl;
	std::cout << gnbest.getPenalty() << std::endl;*/

	data.printNodeInfo(&gnbest);

}
