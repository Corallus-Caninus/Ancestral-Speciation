#include "genome.h"
#include <random>
using namespace std;
//PS: need to parallelize forward_prop.
//	  all other operations can be coarse grain
//	  parallelized within genepool.
//
//	  this development eventually leads into opencl.
//	  best would be to find an underlying representation
//	  (serialization data format) and allow different 
//	  implementations to communicate over MPI.

void genome::mutate_node(mt19937 &twister) {
	uniform_int_distribution<> rconnection(0,edge_count-1);
	uniform_real_distribution<> rconnection_weight(-1, 1);

	edge* selection = edges[rconnection(twister)];
	add_node(selection, float(rconnection_weight(twister)));
}

void genome::mutate_connection(mt19937 &twister) {
	//check if topology is fully connected
	//this allows mutate_connections to always be called
	// safely just as mutate_node.
	int max_connections=1;

	//TODO: if considering recurrent does combination
	//		account for full topology? !count+count?
	for (int i = 2; i <= node_count-1;i++) {
		max_connections *= i;
	}
	if (max_connections == edge_count-1) {
		return; // cannot connect more
	}
	
	uniform_int_distribution<> rin(0,node_count-1);
	uniform_int_distribution<> rout(0,node_count-1);

	uniform_real_distribution<> rconnection_weight(-1, 1);
	node* cin_node = nodes[rin(twister)];
	node* cout_node = nodes[rout(twister)];
	float weight = float(rconnection_weight(twister));

	bool match=false;
	while(true){
		//TODO: sample with replacement.
		cin_node = nodes[rin(twister)];
		cout_node = nodes[rout(twister)];

		//check if connection exists
		for (int i = 0; i < cin_node->num_in_edges;i++) {
			for (int i = 0; i < cout_node->num_in_edges;i++) {
				if (cin_node->out_edges[i]->innovation != 
					cout_node->in_edges[i]->innovation) {
					match = true;
					goto breakall;
				}
			}
		}
	} 
	breakall:
	add_connection(cin_node, cout_node, weight);
}
