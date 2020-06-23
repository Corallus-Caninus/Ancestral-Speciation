#include "network.h"
#include "layer.h"
#include <iostream>
#include <random>

using namespace std;

//TODOPS: optimize further later after functioning 
//	    such as contiguous path addressing and 
//	    reducing pointers.

//TODO: remove network once inheritance is implemented.
network::network() {
	//TODO: many uninitialized locals is bad form.
}
//NOTE: this signifies (and coincides with what was
//		read from k.stanley) that a network can be
//		represented by just its edges. consider developing
//		a serialization form of edges with nodeId's instead of
//		node pointers and load similar to this copy.
network::network(network *copy) {
	//copy by value
	input_dimension = copy->input_dimension;
	output_dimension = copy->output_dimension;

	//allocated nodes and edges
	input_nodes = new node * [input_dimension];
	output_nodes = new node * [output_dimension];
	nodes = new node * [copy->node_count];
	edges = new edge * [copy->edge_count];
	//node_count and edge_count are iterated during
	//copy-construction and shouldnt be statically 
	//initialized 
	node_count = 0;
	edge_count = 0;

	//TODO: can create empty edges with innovations
	//		then attach nodes by innovation in 
	//		node copy constructor.

	//copy by edges, creating nodes if they dont exist
	for (int i = 0; i < copy->edge_count; i++) {
		edge* copy_edge = copy->edges[i];
		//new nodes for this network.
		node* in_node;
		node* out_node;

		//if copy_edge in_node node Id or copy_edge
		//out_node node Id isn't in nodes[] create it
		//and add it else pass to edge_solution constructor.

		check_node(copy_edge, true, in_node);
		check_node(copy_edge, false, out_node);

		edge* edge_solution = new edge(copy_edge);
		//use default copy constructor
		//TODO: copy innovation, recurrent, weight and enabled.
		//TODO: DEPRECATED
		//edge_solution = copy_edge;

		//overwrite pointers assigned from copy_edge to locals
		//all other edge data is pertinent
		edge_solution->in_node = in_node;
		edge_solution->out_node = out_node;
		in_node->add_out_edge(edge_solution);
		out_node->add_in_edge(edge_solution);
		edges[edge_count] = edge_solution;
		edge_count++;
	}
	//set extrema nodes
	//TODO: copy by nodeId verification.
	//this could be refactored into a function call but why
	//copy-construct input_nodes
	for (int i = 0; i < copy->input_dimension; i++) {
		for (int j = 0; j < node_count; j++) {
			if (nodes[j]->nodeId == copy->input_nodes[i]->nodeId) {
				input_nodes[i] = nodes[j];
				break;
			}
		}
	}
	//copy-construct output_nodes
	for (int i = 0; i < copy->output_dimension; i++) {
		for (int j = 0; j < node_count; j++) {
			if (nodes[j]->nodeId == copy->output_nodes[i]->nodeId) {
				output_nodes[i] = nodes[j];
				break;
			}
		}
	}
}

void network::check_node(edge* copy_edge, bool in_node, node*& target_node)
{
	int copy_target;
	if (in_node) {
		copy_target = copy_edge->in_node->nodeId;
	}
	else {
		copy_target = copy_edge->out_node->nodeId;
	}
	bool exists = false;
	int placer = 0;

	for (int j = 0; j < node_count; j++) {
		if (nodes[j]->nodeId == copy_target) {
			exists = true;
			placer = j;
			break;
		}
	}
	if (!exists) {
		target_node = new node(copy_target);
		nodes[node_count] = target_node;
		
		//only increment if novel.
		node_count++;
	}
	else {
		target_node = nodes[placer];
		exists = false;
	}
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
	//TODO: this removes objects from heap with proper
	//		destructors.
	for (int i = 0; i < node_count; i++) {
		delete nodes[i];
	}
	for (int i = 0; i < edge_count; i++) {
		delete edges[i];
	}

	delete[] nodes;
	delete[] edges;
	delete[] input_nodes;
	delete[] output_nodes;
	//NOTE: twister is not deleted since it is referenced
	//		and handled by implementation.
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
	//TODO: these connections arent being added in
	//		add_connection operations
	node* in_node = split_connection->in_node;
	node* out_node = split_connection->out_node;
	split_connection->enabled=false;

	// construct new node
	node* new_node = new node(node_count);
	add_connection(in_node, new_node, weight);
	add_connection(new_node, out_node, 1.0f);

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

//TODO: (post-shave) implement layer data structure. 
//		RB-tree vs DLL vs hash-table vs array
//		considerations are.
//TODO: (post-shave) use singly linked list for buffer
//		since iterating in one direction each time. This
//		is better than contiguous memory since expecting to insert
//		and delete >> access (insertion and deletion is guarantee
//		each propagation step else FSM is locked by definition).
float* network::forward_propagate(float* input_vector) {
	//result vector
	float* outputs = new float[output_dimension];
	//next layer of nodes being forward propagated
	layer step;
	//whether a recurrent signal has been detected.
	bool recurrence_detected = false;
	//size of nodes returned from activation
	int incoming_size = 0;
	//size of current layer

	//TODO: (post shave)inline remove with update since always called together 
	//initialize network
	for (int i = 0; i < input_dimension; i++) {
		//node** init = &nodes[i];
		node** init = &input_nodes[i];
		step.update(init, 1);
	}
	for (int i = 0; i < input_dimension; i++) {
		node** response = input_nodes[i]->activate(input_vector[i],
			incoming_size, max_cycle, recurrence_detected);
		step.remove(nodes[i]);
		step.update(response, incoming_size);
	}
	node** current = step.copy_buffer();
	int current_size = step.buffer_size;

	//loop to complete propagation
	while (true) {
		//for (int i = 0; i < step.buffer_size; i++) {
		for (int i = 0; i < current_size; i++) {
			//prevent output nodes from premature activation.
			if (check_output(current[i])) {
				continue;
			}
			else {
				//TODO: doesnt throw at last entry
				node** response = current[i]->activate(
					incoming_size, max_cycle, recurrence_detected);
				if (recurrence_detected) {
					step.reset_counters();
					recurrence_detected = false;
				}
				step.remove(current[i]); 
				step.update(response, incoming_size);
			}
		}
		delete[] current;
		current = step.copy_buffer();
		current_size = step.buffer_size;
		if (step.final_layer(output_nodes, output_dimension)) {
			break;
		}
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

//TODO: (post-shave)can also set loaded to false but algorithmically should
//		be done during propagation since visiting every node.
void network::reset_nodes() {
	for (int i = 0; i < node_count; i++) {
		nodes[i]->activated = false;
		for (int j = 0; j < nodes[i]->num_out_edges; j++) {
			nodes[i]->out_edges[j]->loaded = false;
		}
		for (int j = 0; j < nodes[i]->num_in_edges; j++) {
			nodes[i]->in_edges[j]->loaded = false;
		}
	}
}