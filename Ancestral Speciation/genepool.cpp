#include "genepool.h"
#include <iostream>
using namespace std;

genepool::genepool(int inputs, int outputs, mt19937& twister, 
				   int set_population, float set_pressure,
				   float set_add_connection, float set_add_node, 
				   float connection_rate, float weight_rate) {
	input_dimension = inputs;
	output_dimension = outputs;

	pressure = set_pressure;

	add_connection_mutation_rate = set_add_connection;
	add_node_mutation_rate = set_add_node;
	connection_mutation_rate = connection_rate;
	weight_mutation_rate = weight_rate;

	population = set_population;
	pool = new genome*[population];

	for (int i = 0; i < population; i++) {
		pool[i] = new genome(inputs, outputs, twister);
	}
}
genepool::~genepool() {
	delete[] pool;
}

//TODO: debug leaks from here down.
void genepool::update(mt19937 &twister) {
	//calculate average fitness
	float avg = 0;
	for (int i = 0; i < population; i++) {
		avg += pool[i]->fitness;
	}
	avg /= population;
	float threshold = avg * pressure;

	//cull lower fitness from pool
	genome** newPool = new genome * [population];
	int keptGenomes = 0;
	//TODO: deleting all genomes if under threshold. flip logic.
	//		is equal to for 0 condition
	for (int i = 0; i < population; i++) {
		if (pool[i]->fitness >= threshold) {
			newPool[keptGenomes] = pool[i];
			keptGenomes++;
		}
		//cleanup culled genomes 
		else {
			delete pool[i];
		}
	}
	
	//TODO: throwing here in copy-constructor
	random_device rd;
	uniform_int_distribution<> selection(0, keptGenomes-1);
	//int remaining = population - keptGenomes;
	for (int i = keptGenomes; i < population; i++) {
		newPool[i] = new genome(newPool[selection(twister)]);
	}
	
	//update pool
	delete[] pool;
	pool = new genome * [population];
	for (int i = 0; i < population; i++) {
		pool[i] = newPool[i];
	}

	delete[] newPool;
	
	//add connection, node and weight mutations.
	uniform_real_distribution<> mutate(0, 1);
	for (int i = 0; i < population; i++) {
		
		if (mutate(twister) > add_connection_mutation_rate) {
			pool[i]->mutate_connection(twister);
		}
		if (mutate(twister) > add_node_mutation_rate) {
			pool[i]->mutate_node(twister);
		}
		if (mutate(twister) > connection_mutation_rate) {
			//TODO: I dont like reaching down this far.
			for (int c = 0; c < pool[i]->net->edge_count; c++) {
				if (mutate(twister) > weight_mutation_rate) {
					pool[i]->mutate_weight(twister, c);
				}
			}
		}
	}
}
