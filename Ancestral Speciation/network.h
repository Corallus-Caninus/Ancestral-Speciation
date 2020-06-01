#pragma once
#include "node.h"
#include "edge.h"
//abstract tree class that handles
//basic topology operations
//implements NEAT operations.

//NOTE: point to edges that way edges dont get
//		moved around when dynamic movements occur.
//		edges must belong solely to nodes for safe
//		memory ops.
//NOTE: this means that network is responsible for
//		storing and/or cleanup of nodes and connection
//		objects on the heap.
//	for now, deallocate all node and edges on
//	deconstruction. expect innovations storage 
//	and serialization to occur during the objects 
//  lifetime.


class network
{
public:
	//network();
	network(int inputs, int outputs);
	~network();

	node** nodes; //node objects
	int node_count = 0;

	// NOTE: edges exist in node objects to prevent
	//		 loss of address during reallocation.
	//		 are pointed to here for ease of access
	//		 in topology operations.
	edge** edges; //edge pointers
	int innovation = 0;

	/// <summary>
	/// add a node to this network by splitting
	/// a connection as per k.stanley's augmentation
	/// operation.
	/// </summary>
	/// <param name="in_node"></param>
	/// <param name="out_node"></param>
	/// <param name="nodes"></param>
	void add_node(node *in_node,node *out_node, float weight);
	/// <summary>
	/// add a connection between two nodes to this 
	/// network
	/// </summary>
	/// <param name="in_node"></param>
	/// <param name="out_node"></param>
	void add_connection(node *in_node, node *out_node,
		float weight);

	//TODO: PROTOTYPES
	//storage representation of network. this
	//can be adjacency matrix if a denser
	//representation cant be found.
	int* serialize(); 
	void remove_connection();
	void remove_node();

	/// <summary>
	/// convert from tree to adjacency matrix
	/// </summary>
	/// <returns></returns>
	int* to_adjacency();
	// TODO: this should override default
	//		constructor. this makes no sense
	//		unless function outside class scope.
	//network* from_adjacency();
};

