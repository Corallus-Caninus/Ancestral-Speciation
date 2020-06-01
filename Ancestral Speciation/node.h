#pragma once
#include "edge.h"
// NOTE:Since we will be propagating >> altering 
//		topology, arrays are better
//		than linked lists.
// NOTE: out_edges and out_node_weights etc.
//		 are associated by index. must be strictly
//		 altered therefor should be private
// TODO: add innovation value to edges
class node
{
public:
	node();
	node(int nodeId = 0);
	~node();
	/// <summary>
	/// nodeId associated with this node
	/// </summary>
	/// <returns></returns>
	int nodeId=0; 
	/// <summary>
	/// outgoing edges from this vertex
	/// </summary>
	edge* out_edges=nullptr;
	int num_out_edges=0;
	/// <summary>
	/// incoming edges to this vertex
	/// </summary>
	edge* in_edges=nullptr;
	int num_in_edges=0;

	// network class is responsible for 
	// symmetry of node/edge operations (pointers).

	void add_in_edge(edge&);
	void add_out_edge(edge&);
	void remove_in_edge(edge&);
	void remove_out_edge(edge&);
};

