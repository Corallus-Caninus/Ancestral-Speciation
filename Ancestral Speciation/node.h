#pragma once
#include "edge.h"
// NOTE:Since we will be propagating >> altering 
//		topology, arrays are better
//		than linked lists.
// NOTE: out_edges and out_node_weights etc.
//		 are associated by index. must be strictly
//		 altered therefor should be private
class node
{
public:
	node();
	node(int nodeId);
	~node();

	/// <summary>
	/// nodeId associated with this node
	/// </summary>
	/// <returns></returns>
	int nodeId=0; 
	/// <summary>
	/// whether this node has been 
	/// activated during propagation.
	/// </summary>
	bool activated = false;
	/// <summary>
	/// outgoing edges from this vertex
	/// </summary>
	edge** out_edges=nullptr;
	int num_out_edges=0; 
	/// <summary>
	/// incoming edges to this vertex
	/// </summary>
	edge** in_edges=nullptr;
	int num_in_edges=0;

	// network class is responsible for 
	// symmetry of node/edge operations (pointers).

	// NOTE: add methods allow for parallel edges
	void add_in_edge(edge*);
	void add_out_edge(edge*);
	/* TODO:
	void remove_in_edge(edge&);
	void remove_out_edge(edge&);
	*/

	node** activate(int &, int, bool &);
	//overload for inputs
	//TODO: (post-shave) should probably pass by reference always
	//		to prevent unecessary stack allocation. only pass by value
	//		when expecting threading.
	node** activate(float, int&, int, bool &);
	//overload for outputs
	float shunt_activate(int&, int, bool&);
	void halt(node**& outputs, int& return_size);
	void propagate(int& num_outputs, node**& outputs, float& sum, int& return_size);

	//TODO: DEPRECATED
	//node** activate(float,int &);
	//float shunt_activate();

	//	overloaded for nodal input_vector, each
	//	input_vector has one respective node in this configuration.
};

