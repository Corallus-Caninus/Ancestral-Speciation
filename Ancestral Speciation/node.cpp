#include "node.h"
#include "assert.h"
#include "math.h"
#include <iostream>
using namespace std;

//TODOPS: chunking node edge array allocation
//		will significantly decrease runtime.
node::node() {
	//default
}
node::node(int set_nodeId) {
	nodeId = set_nodeId;
}
node::~node() {
	//constructed in setter methods
	delete[] out_edges;
	delete[] in_edges;
}

//GETTERS AND SETTERS
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

// PROPAGATION METHODS //

//TODOPS: need to fix this spaghetti.
//TODO: implement disabled connection
//TODO: apply recurrence etc.(reworkish refactor)
//		to input and output activation methods.  
//		this is needed for loaded.
node** node::activate(int &return_size, int max_cycle, bool &detected) {
	//check incoming connections to ensure they are ready
	bool ready_connections=true;
	float sum=0;

	node** outputs;
	int num_outputs = 0;//used to track allocation of output
	//TODO: this is inefficient
	if (num_out_edges == 0) {
		halt(outputs, return_size);
		return outputs;
	}

	//check for recurrent connection timeout
	for (int i = 0;i < num_in_edges;i++) {
		if (in_edges[i]->loaded == false &&
			in_edges[i]->recurrent == false) {
			in_edges[i]->recurrent_counter++;
			if (in_edges[i]->recurrent_counter > max_cycle) {
				in_edges[i]->recurrent = true;
				detected = true;
			}else {
				ready_connections = false;
			}
		}
	}

	if (ready_connections) {
		propagate(num_outputs, outputs, sum, return_size);
		return outputs;
	}else {
		//TODO: ensure this cant lead to a node being reintroduced
		halt(outputs, return_size);
		return outputs;
	}
}

node** node::activate(float incoming, int& return_size, 
						int max_cycle, bool& detected) {
	//check incoming connections to ensure they are ready
	bool ready_connections = true;
	float sum = incoming;

	node** outputs;
	int num_outputs = 0;//used to track allocation of output

	//TODO: this shouldnt happen in initial.
	if (num_out_edges == 0) {
		halt(outputs, return_size);
		return outputs;
	}

	//check for recurrent connection timeout
	for (int i = 0;i < num_in_edges;i++) {
		if (in_edges[i]->loaded == false &&
			in_edges[i]->recurrent == false) {
			in_edges[i]->recurrent_counter++;
			if (in_edges[i]->recurrent_counter > max_cycle) {
				in_edges[i]->recurrent = true;
				detected = true;
			}
			else {
				ready_connections = false;
			}
		}
	}

	if (ready_connections) {
		propagate(num_outputs, outputs, sum, return_size);
		return outputs;
	}
	else {
		//TODO: ensure this cant lead to a node being reintroduced
		halt(outputs, return_size);
		return outputs;
	}
}

//TODO: outputs arent activated so this needs to propagate but be weary of
//		double activation.
float node::shunt_activate(int& return_size, int max_cycle, bool& detected) {
	//check incoming connections to ensure they are ready
	bool ready_connections = true;
	float sum = 0;

	node** outputs;
	int num_outputs = 0;//used to track allocation of output

	/* N/A
	if (num_out_edges == 0) {
		halt(outputs, return_size);
		return outputs;
	}*/

	//TODO: shouldnt have to check recurrence but self loop etc.
	//check for recurrent connection timeout
	for (int i = 0;i < num_in_edges;i++) {
		if (in_edges[i]->loaded == false &&
			in_edges[i]->recurrent == false) {
			in_edges[i]->recurrent_counter++;
			if (in_edges[i]->recurrent_counter > max_cycle) {
				in_edges[i]->recurrent = true;
				detected = true;
			}
			else {
				ready_connections = false;
			}
		}
	}

	//TODO: this should always be ready when called
	//if (ready_connections) {
	//TODO: outputs is unecessary here
	propagate(num_outputs, outputs, sum, return_size);
	delete[] outputs;
	return sum;
		//return outputs; N/A
	//}
	//else {
		//TODO: ensure this cant lead to a node being reintroduced
		//we can do this since checking in network.forward_propagate
		//halt(outputs, return_size);
		//return outputs; 
	//}
}

void node::halt(node**& outputs, int& return_size)
{
	outputs = new node * [1];
	outputs[0] = this; //halt this node for next step.
	return_size = 1;
}

void node::propagate(int& num_outputs, node**& outputs, 
					float& sum, int& return_size)
{
	//calculate size of outputs
	for (int i = 0; i < num_out_edges; i++) {
		if (out_edges[i]->out_node->activated == false) {
			num_outputs++;
		}
	}
	outputs = new node * [num_outputs];
	activated = true;
	for (int i = 0; i < num_in_edges; i++) {
		if (in_edges[i]->loaded == true) {
			sum += in_edges[i]->signal * in_edges[i]->weight;
		}
	}
	//forward propagate softmax squash
	sum *= -1;
	sum = exp(sum);
	sum = 1 / (1 + sum);

	//prepare for next forward propagation.
	for (int i = 0; i < num_in_edges;i++) {
		in_edges[i]->loaded = false;
		in_edges[i]->recurrent_counter = 0;
	}
	int g = 0;
	for (int i = 0;i < num_out_edges;i++) {
		out_edges[i]->signal = sum;
		out_edges[i]->loaded = true;

		//TODO: need to dynamically allocate now and handle null case
		//		can do cheeky operation from last time, counting and adding
		//		based on previous count. saves allocation time.
		if (out_edges[i]->out_node->activated == false) {
			//TODO: can layer.update() handle empty buffer?
			//		it needs to (e.g.: split loop)
			outputs[g] = out_edges[i]->out_node;
			g++;
		}
	}
	return_size = num_outputs;
}


//TODO: DEPRECATED
/*
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
	}

	return_size = num_out_edges;
	return outputs;
}
//TODO: implement disabled connection
float node::shunt_activate() {
	float sum = 0;

	for (int i = 0;i < num_in_edges;i++) {
		assert(in_edges[i]->loaded == true);
		sum += in_edges[i]->signal * in_edges[i]->weight;
	}

	node** outputs = new node * [num_out_edges];
	sum *= -1;
	sum = exp(sum);
	sum = 1 / (1 + sum);

	return sum;
}*/
