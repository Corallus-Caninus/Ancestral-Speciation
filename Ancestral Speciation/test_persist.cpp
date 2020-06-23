#include "genome.h"
#include "persist.h"
#include <random>
#include <iostream>
using namespace std;
//TODO: persist object will exist inside network for
//		method chaining


int main() {
	//initialize
	random_device rd;
	mt19937 twister(rd());
	genome test(3,2,twister);

	for (int i = 0; i < 10; i++) {
		test.mutate_node(twister);
		test.mutate_node(twister);
		test.mutate_connection(twister);
	}

	cout << "mutation complete" << endl;
	persist serial(test.net);
	serial.store("test.net");
	cout << "store network complete" << endl;
	network* retrieval = new network(serial.load("test.NET"));
	cout << "load network complete" << endl;

	persist verify(retrieval);
	verify.store("verify.net");
	delete retrieval;
	//TODO: throwing here network destructor therefor network
	//		possibly not constructed correctly.
	//now compare the two objects
	//compare by edge locals, nodeIds, weight and innovation
	//should be sufficient.
}
