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
	edge(node* set_in_node, node* set_out_node, 
		int set_innovation, float set_weight);
	~edge();
	
	/// <summary>
	/// unit of genetic encoding. used to track
	/// changes throughout evolution.
	/// </summary>
	int innovation; //NOTE: can use sign bit as bool isConstructed
	/// <summary>
	/// the weigth of this connection in the neural
	///	network.
	/// </summary>
	float weight;
	/// <summary>
	/// whether this edge is expressed or not.
	/// </summary>
	bool enabled=true;
	/// <summary>
	/// whether this edge is recurrent
	/// </summary>
	bool recurrent=false; 
	/// <summary>
	/// counter for whether this connection
	/// is recurrent
	/// </summary>
	int recurrent_counter=0;
	/// <summary>
	/// the current signal at this edge
	/// </summary>
	float signal=0;
	/// <summary>
	/// whether this signal is ready for forward step.
	///	signifies whether this edge has been loaded a signal.
	/// allows signals to persist and flips a boolean instead of
	/// wiping a float.
	/// </summary>
	bool loaded=false; 
	/// <summary>
	/// the input node for this connection
	/// </summary>
	node* in_node=nullptr;
	/// <summary>
	/// the output node for this connection
	/// </summary>
	node* out_node=nullptr;
};

