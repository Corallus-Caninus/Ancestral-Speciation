#pragma once
#include "node.h"
#include "edge.h"
#include <random>
using namespace std;

//TODOPS: After priority refactors and feature functional, 
//		  work on some data structure fundamentals to control
//		  allocation cleanly and optimize memory usage. Dont vector
//		  just dont. Need to practicy optimal resource usage.

//TODOPS: lift NEAT specific methods/variables out of network and 
//		nodes/edges for reuse of generic tree class in 
//		ancestral speciation etc. this is the first major
//		refactor once its working. probably last NEAT
//		development before Ancestral Speciation.
//			~do this later I want to shave.~
//	note as trivial as expected, methods currently
//	implement NEAT so would have to extract
//	almost everything. still should do it for 
//	inheritance practice
//
//TODO: implement protected instead of public

//TODOPS: theres some fat that can be trimmed from nodes and edges
//		for a bit/byte or two with signs etc. but this needs to be 
//		justified at scale or when mining with a serializable 
//		representation for long term learning. 
//		until then *S H A A A V E E*

//TODOPS: instead of multithreading forward propagation with recurrent checks
//		  etc, multithread genomic operations and implementations. 
//		  create a genotype->phenotype conversion function for 
//		  fast matrix-mult operations etc. that can be multithreaded 
//		  and/or opencl'd.

//abstract tree class that handles
//basic topology operations
//implements NEAT operations.

class network
{
public:
	//TODO: (post-shave) remove default constructor after inheritance
	network();
	network(int inputs, int outputs, mt19937& set_twister);
	~network();

	//TODO: @DEPRECATED
	int input_dimension;
	int output_dimension;

	//TODOPS: move more things onto stack
	//		  like this where static allocation
	//		  is allowed.
	node** input_nodes;
	node** output_nodes;

	/// <summary>
	/// the largest possible cycle in this
	/// network.
	/// </summary>
	int max_cycle=2; //initialized to init topology

	node** nodes; //node objects
	int node_count = 0;

	edge** edges; //edge pointers
	int edge_count = 0;

	/// <summary>
	/// add a node to this network by splitting
	/// a connection as per k.stanley's augmentation
	/// operation.
	/// </summary>
	/// <param name="split_connection"></param>
	/// <param name="nodes"></param>
	void add_node(edge* split_connection, float weight);
	/// <summary>
	/// add a connection between two nodes to this 
	/// network
	/// </summary>
	/// <param name="in_node"></param>
	/// <param name="out_node"></param>
	void add_connection(node *in_node, node *out_node,
		float weight);

	/// <summary>
	/// forward propagate the network with the given
	/// input vector and return the output vector.
	/// </summary>
	/// <param name="input_vector"></param>
	/// <returns>output_vector</returns>
	float* forward_propagate(float*);

	bool check_output(node* previous);

	/// <summary>
	/// set all nodes to unactivated for a new forward propagation.
	/// </summary>
	void reset_nodes();


	//TODOPS: PROTOTYPES
	//storage representation of network. this
	//can be adjacency matrix if a denser
	//representation cant be found. would prefer
	//custom format based on tree storage. fun exercise.
	// would probably prefer to implement from genom
	// since migrating features up.
	int* serialize(); 
	void remove_connection();
	void remove_node();

	/// <summary>
	/// convert from tree to adjacency matrix
	/// </summary>
	/// <returns></returns>
	int* to_adjacency();
	//network* from_adjacency();
};

