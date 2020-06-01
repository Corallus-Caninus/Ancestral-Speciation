#include "edge.h"

//TODO: need to have edge pointers to nodes

edge::edge() {
	//default
}
edge::edge(int set_innovation, float set_weight) {
	innovation = set_innovation;
	weight = set_weight;
	enabled = true;
}
edge::edge(int set_innovation, float set_weight, 
			bool set_enabled) {
	innovation = set_innovation;
	weight = set_weight;
	enabled = set_enabled;
}
edge::edge(node* set_in_node, node* set_out_node,
		int set_innovation, float set_weight, 
			bool set_enabled) {
	in_node = set_in_node;
	out_node = set_out_node;
	innovation = set_innovation;
	weight = set_weight;
	enabled = set_enabled;
}
edge::~edge() {
	//TODO: may want to remove connections here too
	//		but more likely to keep due to innovation
	//		minning
	delete[] in_node;
	delete[] out_node;
}


