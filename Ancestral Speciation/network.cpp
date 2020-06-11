#include "network.h"
#include "layer.h"
#include <iostream>
#include <random>

using namespace std;

//TODOPS: optimize further later after functioning 
//	    such as contiguous path addressing and 
//	    reducing pointers.

network::network() {

}
network::network(int inputs, int outputs, mt19937& set_twister) {
	//setup randomizers
	mt19937 twister = set_twister; 
	//TODO: should this also be passed by reference?
	uniform_real_distribution<> redge_weight(-1,1);
	input_dimension = inputs;
	output_dimension = outputs;

	nodes = new node*[inputs + outputs];
	edges = new edge*[inputs * outputs];

	input_nodes = new node*[inputs];
	output_nodes = new node*[outputs];

	node_count = inputs+outputs;

	//build input topology
	for (int i = 0;i < inputs;i++) {
		nodes[i] = new node(i);
		input_nodes[i] = nodes[i];
	}
	//build output topology
	for (int i = inputs; i < inputs+outputs; i++) {
		nodes[i] = new node(i);
		output_nodes[i-inputs] = nodes[i];
	}
	//link and build
	for (int i = 0; i < inputs; i++) {
		for (int j = inputs; j < inputs+outputs;j++) {
			float weight = float(redge_weight(twister));

			edge* new_edge = new edge(
				nodes[i], nodes[j],
				edge_count, weight);
			edges[edge_count] = new_edge;
			edge_count += 1;

			nodes[i]->add_out_edge(new_edge);
			nodes[j]->add_in_edge(new_edge);
		}
	}
}
network::~network() {
	delete[] nodes;
	delete[] edges;
	delete[] input_nodes;
	delete[] output_nodes;
}

//TODO:  add_node and add_edge require innovation
//		 global check method. This can occur
//		 after node is added at some
//		 latter operation. later operation is better
//		 since is necessary for async islands updating
//		 RoM.
//	currently this is handled outside of network.
//	innovation is constructed as uninitialized.
void network::add_node(edge* split_connection, float weight) {
	node* in_node = split_connection->in_node;
	node* out_node = split_connection->out_node;
	split_connection->enabled=false;

	// construct new node
	node* new_node = new node(node_count);
	add_connection(in_node, new_node, weight);
	add_connection(new_node, out_node, 1.0);

	//add fully constructed node to network
	node** node_buffer = new node*[node_count];
	for (int i = 0; i < node_count;i++) {
		node_buffer[i] = nodes[i];
	}
	delete[] nodes;

	node_count += 1;
	nodes = new node*[node_count];
	for (int i = 0; i < node_count - 1;i++) {
		nodes[i] = node_buffer[i];
	}
	nodes[node_count-1] = new_node;
	delete[] node_buffer;

	//update largest possible cycle
	max_cycle = node_count + 1;
}

//TODO: edge_count is associated here with innovation.
//		resolve this. should use innovation as a local
//		vs global value and not edge_count, so when innovations
//		are updated, count from newest innovation as best guess
//		between global innovation updates.
void network::add_connection(node* in_node, node* out_node,
	float weight) {
	//initialize new connection
	edge_count += 1;
	edge* new_edge = new edge(
		in_node, out_node,
		edge_count, weight);

	//TODO: shouldnt dereference here as need to share ptr
	in_node->add_out_edge(new_edge);
	out_node->add_in_edge(new_edge);

	//now add edge pointers to network
	edge** edge_buffer = new edge*[edge_count-1];
	for (int i = 0;i < edge_count-1;i++) {
		edge_buffer[i] = edges[i];
	}
	delete[] edges;
	
	edges = new edge*[edge_count];
	for (int i = 0;i < edge_count - 1;i++) {
		edges[i] = edge_buffer[i];
	}
	edges[edge_count-1] = new_edge;
	delete[] edge_buffer;
}

//TODOPS: implement layer data structure. 
//		RB-tree vs DLL vs hash-table vs array
//		considerations are strictly PS.
float* network::forward_propagate(float* input_vector) {
	//result vector
	float* outputs = new float[output_dimension];
	//current layer of nodes being forward propagated
	layer step;
	//whether a recurrent signal has been detected.
	bool recurrence_detected = false;
	//size of nodes returned from activation
	int incoming_size = 0;
	//size of previous layer
	int previous_size = 0;
	//TODO: this relies on nodes structure, nodes{0,input_dimension} must
	//		be input nodes. This isnt bad and such format tricks should
	//		be implemented elsewhere for efficiency (post-shave) unless
	//		future features (i.e.: pruning) are planned to be implemented.
	for (int i = 0; i < input_dimension; i++) {
		node** response = nodes[i]->activate(
			incoming_size, max_cycle, recurrence_detected);
		step.update(response, incoming_size);
	}
	node** previous = step.copy_buffer();
	previous_size = step.buffer_size;

	//TODO: throwing extrema activation in hidden layer.
	//		simple fix: if is output dont activate at all here.
	//		requires check.. optimization is post-shave
	while (true) {
		for (int i = 0; i < step.buffer_size; i++) {
			//prevent output nodes from premature activation.
			if (check_output(previous[i])) {
				continue;
			}
			else {
				node** response = previous[i]->activate(
					incoming_size, max_cycle, recurrence_detected);
				if (recurrence_detected) {
					step.reset_counters();
					recurrence_detected = false;
				}
				step.remove(previous[i]);
				step.update(response, incoming_size);
			}
			if (step.final_layer(output_nodes, output_dimension)) {
				break;
			}

			delete[] previous;
			previous = step.copy_buffer();
			previous_size = step.buffer_size;
		}

		//harvest output signals
		for (int i = 0; i < output_dimension; i++) {
			//TODO: (post-shave) shouldnt need to pass so much to this method.
			outputs[i] = output_nodes[i]->shunt_activate(
				incoming_size, max_cycle, recurrence_detected);
		}
		//refresh the network
		reset_nodes();
		return outputs;
	}
}

bool network::check_output(node* previous)
{
	for (int j = 0; j < output_dimension; j++) {
		if (output_nodes[j] == previous) {
			//skip this node.
			return(true);
		}
	}
	return(false);
}

//TODO: can also set loaded to false but algorithmically should
//		be done during propagation since visiting every node.
void network::reset_nodes() {
	for (int i = 0; i < node_count; i++) {
		nodes[i]->activated = false;
	}
}