#include "edge.h"

edge::edge() {}
edge::edge(edge* copy) {
	innovation = copy->innovation;
	weight = copy->weight;
	enabled = copy->enabled;
	recurrent = copy->recurrent;
}
edge::edge(node* set_in_node, node* set_out_node,
		int set_innovation, float set_weight) {
	in_node = set_in_node;
	out_node = set_out_node;
	innovation = set_innovation;
	weight = set_weight;
}
edge::~edge() {
	// NOTE: currently edge doesnt dynamically allocate.
	//delete[] in_node;
	//delete[] out_node;
}
