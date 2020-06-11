#include <iostream>
#include "genome.h"
#include <random>
using namespace std;

//its gonna be a party..
int main() {
	cout << "Hello World!\n";
	random_device rd{};
	//mt19937* twister = new mt19937(rd());
	mt19937 twister(rd());
	genome test(3,2,twister);

	//printout network
	cout << "NODE: " << endl;
	for (int i = 0; i < test.net->node_count; i++) {
		cout << test.net->nodes[i]->nodeId;
	}
	cout << "AND EDGES: " << endl;
	for (int i = 0; i < test.net->edge_count; i ++ ) {
		cout << test.net->edges[i]->innovation;
	}
	cout << endl;
	//test mutations
	for (int i = 0; i < 10; i++) {
		test.mutate_node(twister);
		test.mutate_connection(twister);
	}
	//print out mutated solution
	cout << "NEXT" << endl;
	cout << "NODE: " << endl;
	for (int i = 0; i < test.net->node_count; i++) {
		cout << test.net->nodes[i]->nodeId;
	}
	cout << "AND EDGES: " << endl;
	for (int i = 0; i < test.net->edge_count; i ++ ) {
		cout << test.net->edges[i]->in_node->nodeId;
		cout << "->";
		cout << test.net->edges[i]->out_node->nodeId;
		cout << endl;
	}
	cout << endl;

	//test propagation
	float inputs[3] = { 1.1f, 1.2f, 1.3f };
	float* results = test.net->forward_propagate(inputs);
	cout << results[0] << endl;
	cout << results[1] << endl;
	float* results_two = test.net->forward_propagate(inputs);
	cout << results_two[0] << endl;
	cout << results_two[1] << endl;
	float* results_three = test.net->forward_propagate(inputs);
	cout << results_three[0] << endl;
	cout << results_three[1] << endl;
}