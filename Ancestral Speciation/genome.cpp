#include "genome.h"
#include "math.h"
#include <random>
#include <iostream>
//#include "network.h"
using namespace std;
genome::genome(int inputs, int outputs, mt19937 &twister){
	//:network(inputs, outputs, twister){
	net = new network(inputs, outputs, twister);
}
genome::~genome() {
	delete net;
}

void genome::mutate_node(mt19937 &twister) {
	uniform_int_distribution<> rconnection(0,net->edge_count-1);
	uniform_real_distribution<> rconnection_weight(-1, 1);

	edge* selection = net->edges[rconnection(twister)];
	net->add_node(selection, float(rconnection_weight(twister)));
}

bool genome::mutate_connection(mt19937& twister) {
	//check if topology is fully connected
	//this allows mutate_connections to always be called
	// safely just as mutate_node.
	int max_connections=1;

	//TODO: if considering recurrent does combination
	//		account for full topology? !count+count?
	for (int i = 2; i <= net->node_count-1;i++) {
		max_connections *= i;
	}
	max_connections += net->node_count; //consider loop connections
	if (max_connections == net->edge_count-1) {
		return false; // cannot connect more
	}
	
	uniform_int_distribution<> rin(0, net->node_count - 1);
	uniform_int_distribution<> rout(0, net->node_count - 1);
	uniform_real_distribution<> rconnection_weight(-1, 1);
	int num = net->nodes[1]->out_edges[1]->innovation;

	while (true) {
		node* cin_node = net->nodes[rin(twister)];
		node* cout_node = net->nodes[rout(twister)];

		float weight = float(rconnection_weight(twister));

		//TODO: (post-shave) sample with replacement for more efficient 
		//		selection and entropy consumption at scale
		//check if connection exists
		//TODO: 
		for (int i = 0; i < cin_node->num_in_edges;i++) {
			for (int j = 0; j < cout_node->num_out_edges;j++) {
				if(cin_node->in_edges[i]->innovation != 
					cout_node->out_edges[j]->innovation){
					net->add_connection(cin_node, cout_node, weight);
					return true;
				}
			}
		}
	}
}
