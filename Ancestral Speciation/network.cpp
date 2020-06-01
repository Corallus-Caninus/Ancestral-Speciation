#include <random>
#include "network.h"
#include "assert.h"

//TODO: since implementing random in constructor,
//		should implement in add_node and add_edge
//		respectively to keep seed consistent across
//		the operations distributions 
//				(DONT UNDERESTIMATE THIS)

// NOTE: constructed topology is required for
//		 all operations listed here. 

//TODO: losing passed in edge objects in
//		constructor.
//		can either construct edge
//		in add_*_edge and recurse to
//		oposing node or pass by value
//		and resolve same pointer address
//		otherwise/with 
//		innovation (sloppy and slow traversal)
//		edge pointers are fastest traversal
//		but have reallocation problems.
//
//	can point nodes to nodes with an associated
//  float value by index (two arrays)
//	fast traversal but still not reallocatable 
//
// can also rebuild node->node pointers
// after each reallocation by walking the
// topology using innovations, building
// pointers. C++ why..
//
// can always try DLinked List for dynamic allocation
//
// 1. can allocate objects on the heap
//    but not in array and point to them.
//	  reallocation wont change anything but
//    pointer addresses and can delete dereferenced
//    pointers to objects if needing to cleanup heap 
//    objects pointed to. implement this with edge 
//	  pointers for fast traversal. pointers for the win.
//	  node point to edge and edge point to 
//	  node (ablate in_edge)? maybe. in_edge is useful
//	  for signal accumulation although this can be
//	  handled with mutating node local variable 
//	  called from edge.
//
//	  optimize further later after functioning 
//	  such as contiguous path addressing and 
//	  reducing pointers.
//			~sleep on it~
//		      attempt 1.
//		..sounds like professor rework..
//		 at least its limited to network 
//		 (shoutout object encapsulation)

network::network(int inputs, int outputs) {
	nodes = new node*[inputs + outputs];
	edges = new edge*[inputs * outputs];
	node_count = inputs+outputs;

	//build input topology
	for (int i = 0;i < inputs;i++) {
		nodes[i] = new node(i);
	}
	//build output topology
	for (int j = inputs; j < inputs+outputs; j++) {
		nodes[j] = new node(j);
	}
	//link and build
	for (int i = 0; i < inputs; i++) {
		for (int j = inputs; j < inputs+outputs;j++) {
			//TODO: seed and select PRNG also this biases
			//		distribution..
			float weight = float(double(rand()) / double((RAND_MAX))) -
				float(double(rand())/double(RAND_MAX));

			edge* new_edge = new edge(innovation, weight);
			edges[innovation] = new_edge;
			innovation += 1;

			nodes[i]->add_out_edge(*new_edge);
			nodes[j]->add_in_edge(*new_edge);
		}
	}
}
network::~network() {
	//remove objects from the heap
	for (int i = 0;i < node_count;i++) {
		delete nodes[i];
	}
	for (int i = 0; i < innovation;i++) {
		delete edges[i];
	}
	delete[] nodes;
	delete[] edges;
}

//TODO:  add_node and add_edge require innovation
//		 global check method. this can either happen
//		 inline or after node is added at some
//		 latter operation. later operation is better
//		 since is necessary for async islands updating
//		 RoM.
//TODO: need to protect calls to add_node to ensure
//		passed in nodes are connected.

void network::add_node(node *in_node, node *out_node, 
	float weight) {
	//find the connection between nodes and disable it.
	for (int i = 0; i < in_node->num_out_edges; i++) {
		for (int j = 0;j < out_node->num_in_edges;j++) {
			if (in_node->out_edges[i].innovation ==
				out_node->in_edges[j].innovation) {
				//both point to same edge so just
				//pick one
				in_node->out_edges[j].enabled = false;
				goto label; //break all
			}
		}
	}
	label:

	node* new_node = new node(node_count);
	innovation += 1;
	edge* in_edge = new edge(innovation, weight);
	innovation += 1;
	edge* out_edge = new edge(innovation, 1.0);

	new_node->add_in_edge(*in_edge);
	new_node->add_out_edge(*out_edge);
	in_node->add_out_edge(*in_edge);
	out_node->add_in_edge(*out_edge);

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

	//now add edge pointers
	edge** edge_buffer = new edge*[innovation-2];
	for (int i = 0;i < innovation-2;i++) {
		edge_buffer[i] = edges[i];
	}
	delete[] edges;
	
	edges = new edge*[innovation];
	for (int i = 0;i < innovation - 2;i++) {
		edges[i] = edge_buffer[i];
	}
	edges[innovation - 2] = in_edge;
	edges[innovation - 1] = out_edge;
	delete[] edge_buffer;
}

//TODO: need to ensure not already connected
void network::add_connection(node* in_node, node* out_node,
	float weight) {
	//initialize new connection
	innovation += 1;
	edge new_edge = edge(innovation, weight);

	in_node->add_out_edge(new_edge);
	out_node->add_in_edge(new_edge);

	//now add edge pointers
	edge** edge_buffer = new edge*[innovation-1];
	for (int i = 0;i < innovation-1;i++) {
		edge_buffer[i] = edges[i];
	}
	delete[] edges;
	
	edges = new edge*[innovation];
	for (int i = 0;i < innovation - 1;i++) {
		edges[i] = edge_buffer[i];
	}
	edges[innovation-1] = &new_edge;
	delete[] edge_buffer;
}
