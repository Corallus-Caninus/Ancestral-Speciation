#include "node.h"
#include "assert.h"
#include "math.h"
#include <iostream>
using namespace std;

//TODO: chunking node edge array allocation
//		may significantly decrease runtime.
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

//GETTERS AND SETTERS
//TODO: may need to changge to array of pointers
//		to ensure nodes share edge
void node::add_in_edge(edge *in_edge) {
	if (num_in_edges == 0) {
		// initialize in_edges array
		num_in_edges = 1;
		in_edges = new edge*[1];
		in_edges[0] = in_edge;
	} else{
		// copy old edge data structure into buffer
		edge** edge_buffer = new edge*[num_in_edges];
		for (int i = 0;i < num_in_edges;i++) {
			edge_buffer[i] = in_edges[i];
		}
		delete[] in_edges;

		// write in buffer to new edge data structure
		num_in_edges += 1;
		in_edges = new edge*[num_in_edges];
		for (int i = 0; i < num_in_edges-1; i++){
			in_edges[i] = edge_buffer[i];
		}
		in_edges[num_in_edges-1] = in_edge;
		delete[] edge_buffer;
	}
}
void node::add_out_edge(edge *out_edge) {
	if (num_out_edges == 0) {
		// initialize out_edges array
		num_out_edges = 1;
		out_edges = new edge*[1];
		out_edges[0] = out_edge;
	} else {
		// copy old edge data structure into buffer
		edge** edge_buffer = new edge*[num_out_edges];
		for (int i = 0;i < num_out_edges;i++) {
			edge_buffer[i] = out_edges[i];
		}
		delete[] out_edges;

		// write out buffer to new edge data structure
		num_out_edges += 1;
		out_edges = new edge*[num_out_edges];
		for (int i = 0; i < num_out_edges - 1; i++) {
			out_edges[i] = edge_buffer[i];
		}
		out_edges[num_out_edges-1] = out_edge;
		delete[] edge_buffer;
	}
}
/*
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
*/

// PROPAGATION METHODS

//TODOPS: need to fix this spaghetti.
node** node::activate(int &return_size) {
	//TODO: pointing wrong after step.
	//check incoming connections to ensure they are ready
	bool ready_connections=true;
	float sum=0;

	cout << "IN NODE ACTIVATION: " << nodeId << endl;

	//TODO: raviolli
	if (num_out_edges == 0) {
		node** outputs = new node * [1];
		outputs[0] = this; //halt this node for next step.

		cout << "HIDDEN ACTIVATION: halting self " << outputs[0]->nodeId << endl;

		return_size = 1;
		return outputs;
	}

	for (int i = 0;i < num_in_edges;i++) {
		//ready_connections+=int(in_edges[i].loaded);
		if (in_edges[i]->loaded == false && 
			in_edges[i]->recurrent == false) {
			ready_connections = false;
			cout << "NODE ACTIVATION recurrent at: " 
				<< in_edges[i]->innovation << endl;
			cout << in_edges[i]->in_node->nodeId << endl;
			cout << in_edges[i]->loaded;
			in_edges[i]->recurrent_counter++;
		}else{
		//TODO: DEPRECATED
		//if (in_edges[i].loaded == true) {
		// ignore unready recurrent connections
			sum += in_edges[i]->signal;
			//in_edges[i].loaded = false;
		}
	}

	if (ready_connections) {
		node** outputs = new node*[num_out_edges];
		//cycles = 0;//activated
		//forward propagate softmax squash
		sum *= -1;
		sum = exp(sum);
		sum = 1 / (1 + sum);

		//prepare for next forward propagation.
		//TODO: broken here
		for (int i = 0; i < num_in_edges;i++) {
			in_edges[i]->loaded = false;
			in_edges[i]->recurrent_counter = 0;
		}
		for (int i = 0;i < num_out_edges;i++) {
			out_edges[i]->signal = sum;
			out_edges[i]->loaded = true;
			outputs[i] = out_edges[i]->out_node;
		}
		return_size = num_out_edges;
		return outputs;
	}else {
		//TODO: excessive alloc calls
		//		this may be necessary due to
		//		dynamic nature.
		//delete[] outputs; 		
		node** outputs = new node*[1];
		//ummmm wat
		outputs[0] = this; //halt this node for next step.
		return_size = 1;
		return outputs;
	}
}

node** node::activate(float init, int &return_size) {
	//TODO: check recurrence here.

	//check incoming connections to ensure they are ready
	bool ready_connections = true;
	float solution = 0;
	//WARNING: Memory Leak: must be destroyed in caller.
	node** outputs = new node*[num_out_edges];

	//forward propagate softmax
	init *= -1;
	solution = exp(init);
	solution = 1 / (1 + solution);
	for (int i = 0;i < num_out_edges;i++) {
		out_edges[i]->signal = solution;
		out_edges[i]->loaded = true;
		outputs[i] = out_edges[i]->out_node;
		cout << "initialize prop to node: "<< outputs[i]->nodeId << endl;
	}

	return_size = num_out_edges;
	return outputs;
}