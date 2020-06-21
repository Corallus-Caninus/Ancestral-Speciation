#include "node.h"
#include "assert.h"
#include "math.h"
#include <iostream>
using namespace std;

node::node() {
	//default
}
node::node(int set_nodeId) {
	nodeId = set_nodeId;
}
node::~node() {
	//constructed in setter methods
	//NOTE: edges are cleaned up in network 
	//		due to cyclic pointer dependencies
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
		halt(outputs, return_size);
		return outputs;
	}
}

//TODO: this is fine and all but float incoming doesn't also get halted...
//		could make hanging edge for all input nodes. could also force fire
//		all inputs and allow recurrence to be ignored, whatever order occurs
//		is the order it occurs for now..
//TODO: by default connections going to inputs must be labelled recurrent.
//		this should hold for all intra-extrema connections too.
//		for now an "if signal is ready fire policy"
node** node::activate(float incoming, int& return_size, 
						int max_cycle, bool& detected) {
	//check incoming connections to ensure they are ready
	bool ready_connections = true;
	float sum = incoming;

	node** outputs;
	int num_outputs = 0;//used to track allocation of output

	/*
	if (num_out_edges == 0) {
		halt(outputs, return_size);
		return outputs;
	}*/
	/*
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
	}*/

	//if (ready_connections) {
	propagate(num_outputs, outputs, sum, return_size);
	return outputs;
	//}
	/*else {
		//TODO: ensure this cant lead to a node being reintroduced
		halt(outputs, return_size);
		return outputs;
	}*/
}

//TODO: halt shunt_activate to allow proper recursive propagation. 
//		harvest outputs when all outputs in final layer && all outputs
//		activated == true
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

//TODO: doesnt halt therefore doesnt detect recurrence
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

//TODO: output to input connections aren't halted so dont trigger recurrence.
//		either hard code or make considerations
//TODO: sum shouldnt be passed in
void node::propagate(int& num_outputs, node**& outputs, 
					float& sum, int& return_size)
{
	//calculate size of outputs
	for (int i = 0; i < num_out_edges; i++) {
		if (out_edges[i]->out_node->activated == false 
			&& out_edges[i]->recurrent == false) {
			num_outputs++;
		}
	}
	outputs = new node * [num_outputs];
	for (int i = 0; i < num_in_edges; i++) {
		//TODO: doesnt work with recurrence either store
		//		recurrent signals and default others to 0
		//		per forward prop or set in_edges[i]->recurrent = true
		//		and allow default 0 to be summed (initial null effect)
		//TODO: if this doesnt work why do propagation deltas occur with
		//		recurrent connections? write this out on paper. some
		//		connections may fire ahead of a node that receives reccurent
		//		signal.
		if (in_edges[i]->loaded == true || in_edges[i]->recurrent) {
			sum += in_edges[i]->signal * in_edges[i]->weight;
		}
	}
	//forward propagate softmax squash
	//sum *= -1;
	//sum = exp(sum);
	//sum = 1 / (1 + sum);
	//tanh instead of logistic for
	//negation range property
	float expo = exp(sum);
	float recp = exp(-1 * sum);
	sum = (expo - recp) / (expo + recp);

	int g = 0;
	for (int i = 0;i < num_out_edges;i++) {
		out_edges[i]->signal = sum;
		out_edges[i]->loaded = true;

		if (out_edges[i]->out_node->activated == false 
			&& out_edges[i]->recurrent == false) {
			//TODO: can layer.update() handle empty buffer?
			//		it needs to (e.g.: split loop)
			outputs[g] = out_edges[i]->out_node;
			//TODO: this is fine here.
			g++;
		}
	}
	
	//prepare for next forward propagation.
	for (int i = 0; i < num_in_edges;i++) {
		in_edges[i]->loaded = false;
		//DEPRECATED
		in_edges[i]->recurrent_counter = 0;
	}
	activated = true;

	return_size = num_outputs;
}

