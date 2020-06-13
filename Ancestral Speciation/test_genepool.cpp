#include "genepool.h"
#include <random>
#include <iostream>
using namespace std;

int main() {
	const static int scale_out = 10;
	//setup PRNG
	random_device rd;
	mt19937 twister(rd());

	//test copy operation for evaluation
	genome test(3, 2, twister);
	for (int i = 0; i < scale_out; i++) {
		test.mutate_node(twister);
	}
	network copy(test.net);
	cout << "COPY NETWORK ADDRESSING" << endl;
	for (int i = 0; i < copy.edge_count; i++) {
		cout << copy.edges[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < copy.node_count; i++) {
		cout << copy.nodes[i] << " ";
	}
	cout << endl << endl;
	
	cout << "ORIGINAL NETWORK ADRESSING: " << endl;
	for (int i = 0; i < test.net->edge_count; i++) {
		cout << test.net->edges[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < test.net->node_count; i++) {
		cout << test.net->nodes[i] << " ";
	}
	cout << endl;
	cout << copy.node_count;
	cout << " " << test.net->node_count;
	cout << endl << copy.edge_count << " " << test.net->edge_count;



	cout << "Happy Cows" << endl;
}
