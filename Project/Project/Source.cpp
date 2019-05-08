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
	unordered_set<Node> population;
	population.insert(ini);
	gn.solve(population);


}
