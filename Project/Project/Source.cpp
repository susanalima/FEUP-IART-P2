#include <iostream>

#include "HillClimbing.h"

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
}
