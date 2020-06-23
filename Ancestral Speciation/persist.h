#pragma once
#include "network.h"

//TODO: Serialize and Store methods. Serialize stores
//		object with all values for external visualization
//		etc. and store for memory efficient archived storage.
//TODO: each network should have an object instance of transform
//		locally for method chaining.
class persist 
{
public:
	persist(network* input);
	~persist();

	network* target;

	//void serialize(string filename);
	//TODO: should network be allocated on heap
	//		by using copy constructor or
	//		allocated here and assume cleanup
	//		elsewhere. for now assume copy-constructor
	//		because memory leaks.
	//network deserialize(string filename);

	void store(string filename);
	network load(string filename);
	void force_edge(network* solution, edge* proc_edge);
	void force_node(network* solution, node* proc_outNode);
};

