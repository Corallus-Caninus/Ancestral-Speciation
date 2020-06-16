#include "genepool.h"
#include <random>
#include <iostream>
using namespace std;

int main() {
	const static int scale_out = 10;
	//setup PRNG
	random_device rd;
	mt19937 twister(rd());

	//test copy->net->operation for evaluation
	genome *test = new genome(3, 2, twister);
	for (int i = 0; i < scale_out; i++) {
		test->mutate_node(twister);
		test->mutate_connection(twister);
	}
	//network copy->net(test->net);
	genome *copy = new genome(test);
	cout << "COPY NETWORK ADDRESSING" << endl;
	for (int i = 0; i < copy->net->edge_count; i++) {
		cout << copy->net->edges[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < copy->net->node_count; i++) {
		cout << copy->net->nodes[i] << " ";
	}
	cout << endl << endl;
	
	cout << "ORIGINAL NETWORK ADRESSING: " << endl;
	for (int i = 0; i < test->net->edge_count; i++) {
		cout << test->net->edges[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < test->net->node_count; i++) {
		cout << test->net->nodes[i] << " ";
	}
	cout << endl;
	cout << copy->net->node_count;
	cout << " " << test->net->node_count;
	cout << endl << copy->net->edge_count << " " << test->net->edge_count;

	delete copy;
	delete test;

	//TEST GENEPOOL OPERATIONS
	genepool* eval = new genepool(3, 2, twister, 10, 0.5f, 
								0.5f,0.5f,0.5f,0.5f);
	cout << "Cows in the pool: " << endl;
	cout << eval->pool[0] << endl;

	//TEST GENEPOOL EVOLUTION (here we goooo)
	//TODO: when this is done, we are ready to implement and shave
	//		(known bug in extrema forward propagation and numerous 
	//		optimizations but thats not the point of shave goal
	//		deliverables)
	//TODO: little leak still..
	
	for (int j = 0; j < 1000; j++) {
		//spoof fitness
		float fit = 0;
		for (int i = 0; i < eval->population; i++) {
			eval->pool[i]->fitness = fit;
			fit += 1.0f;
			cout << eval->pool[i]->net->node_count << endl;
			cout << eval->pool[i]->net->edge_count << endl;
			cout << "NEXT: " << endl;
			
		}
		cout << endl << "GENERATION" << endl;
		eval->update(twister);
	}
	/*
	for (int j = 0; j < 100000000; j++) {
		genome* lmnop = new genome(3, 2, twister);
		lmnop->mutate_connection(twister);
		lmnop->mutate_node(twister);
		lmnop->mutate_weight(twister, 0);
		//lmnop->net->add_node(lmnop->net->edges[1], 1.1f);
		//lmnop->net->add_connection(lmnop->net->nodes[0], lmnop->net->nodes[1], 1.1f);
		genome* xyz = new genome(lmnop);
		delete lmnop;
		delete xyz;
	}*/
	/*
	//TEST NETWORK LEAK (ablate to isolate source in abstraction heirarchy)
	for (int j = 0; j < 10000; j++) {
		network* lmnop = new network(3, 2, twister);
		lmnop->add_node(lmnop->edges[1], 1.1f);
		lmnop->add_connection(lmnop->nodes[0], lmnop->nodes[1], 1.1f);
		network* xyz = new network(lmnop);
		delete lmnop;
		delete xyz;
	}*/
	cout << "Happy Cows" << endl;
	char delay;
	cin >> delay;
}
