#include "node.h"
#include "assert.h"

// NOTE: these operations require
//		 allocation of nodes and edges
//		 data structure. can implement
//		 allocation here with a simple
//		 if condition to the first operations
//		 in the add_* methods.
// TODO: the above is a bug not a feature.
//		 num_out_edges requires external
//		 initialization based on this logic.

node::node() {
	//default
}
node::node(int set_nodeId) {
	nodeId = set_nodeId;
}
node::~node() {
	delete[] out_edges;
	delete[] in_edges;
}

// TODO: second call to dynamic allocation
//		 throws heap corrupt. likely bad iterator.
void node::add_in_edge(edge& in_edge) {
	if (num_in_edges == 0) {
		// initialize in_edges array
		num_in_edges = 1;
		in_edges = new edge[1];
		in_edges[0] = in_edge;
	} else{
		// copy old edge data structure into buffer
		edge* edge_buffer = new edge[num_in_edges];
		for (int i = 0;i < num_in_edges;i++) {
			edge_buffer[i] = in_edges[i];
		}
		delete[] in_edges;

		// write in buffer to new edge data structure
		num_in_edges += 1;
		in_edges = new edge[num_in_edges];
		for (int i = 0; i < num_in_edges-1; i++){
			in_edges[i] = edge_buffer[i];
		}
		in_edges[num_in_edges-1] = in_edge;
		delete[] edge_buffer;
	}

}

void node::add_out_edge(edge& out_edge) {
	if (num_out_edges == 0) {
		// initialize in_edges array
		num_out_edges = 1;
		out_edges = new edge[1];
		out_edges[0] = out_edge;
	} else {
		// copy old edge data structure into buffer
		edge* edge_buffer = new edge[num_out_edges];
		for (int i = 0;i < num_out_edges;i++) {
			edge_buffer[i] = out_edges[i];
		}
		delete[] out_edges;

		// write out buffer to new edge data structure
		num_out_edges += 1;
		out_edges = new edge[num_out_edges];
		for (int i = 0; i < num_out_edges - 1; i++) {
			out_edges[i] = edge_buffer[i];
		}
		out_edges[num_out_edges-1] = out_edge;
		delete[] edge_buffer;
	}
}

void node::remove_in_edge(edge &removal) {
	edge* edge_buffer = new edge[num_in_edges];
	edge_buffer = in_edges;
	int target = -1;
	//locate the target to be removed
	for (int i = 0;i < num_in_edges;i++) {
		if (in_edges[i].innovation == 
			removal.innovation) {
			target = i;
		}
	}
	assert(target != -1);

	//overwrite the target for repartitioning
	num_in_edges -= 1;
	for (int j = target;j < num_in_edges;j++) {
		edge_buffer[j] = in_edges[j + 1];
	}

	//resize partition 
	delete[] in_edges;
	in_edges = new edge[num_in_edges];
	for (int i = 0; i < num_in_edges;i++) {
		in_edges[i] = edge_buffer[i];
	}
}

void node::remove_out_edge(edge &removal) {
	edge* edge_buffer = new edge[num_out_edges];
	edge_buffer = out_edges;
	int target = -1;
	//locate the target to be removed
	for (int i = 0;i < num_out_edges;i++) {
		if (out_edges[i].innovation == 
			removal.innovation) {
			target = i;
		}
	}
	assert(target != -1);

	//overwrite the target for repartitioning
	num_out_edges -= 1;
	for (int j = target;j < num_out_edges;j++) {
		edge_buffer[j] = out_edges[j + 1];
	}

	//resize partition 
	delete[] out_edges;
	out_edges = new edge[num_out_edges];
	for (int i = 0; i < num_out_edges;i++) {
		out_edges[i] = edge_buffer[i];
	}
}

