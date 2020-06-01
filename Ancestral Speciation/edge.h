#pragma once
//forward declaration is appropriate due to
//intrinsic dependency of nodes and edges.
class node;//forward declaration

// TODO: innovation restructering method
// a structure due to publicly mutable nature
// wrt network.

struct edge
{
	edge();
	edge(int, float);
	edge(int, float, bool);
	edge(node* set_in_node, node* set_out_node, 
		int set_innovation, float set_weight, 
		bool set_enabled);
	~edge();
	
	/// <summary>
	/// unit of genetic encoding. used to track
	/// changes throughout evolution.
	/// </summary>
	int innovation;
	/// <summary>
	/// the weigth of this connection in the neural
	///	network.
	/// </summary>
	float weight;
	/// <summary>
	/// whether this gene is expressed or not.
	///	this differentiates the phenotype
	/// (artificial neural network) from
	/// the genotype (raw network graph)
	/// </summary>
	bool enabled;

	/// <summary>
	/// the input node for this connection
	/// </summary>
	node* in_node=nullptr;
	/// <summary>
	/// the output node for this connection
	/// </summary>
	node* out_node=nullptr;
};

