#include "persist.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

persist::persist(network* input) {
	target = input;
}
persist::~persist() {
	//default destructor
	//network is only dynamic allocated object
	//and is pointed to and handled in calling namespace
}

void persist::store(string filename) {
	//prepare file stream
	ofstream fout;
	fout.open(filename);

	//write out the network.
	//		leaving out recurrent bit because more space 
	//		efficient and would require a char instead of 
	//		bool which is 4x larger.
	for (int i = 0; i < target->edge_count; i++) {
		int innov = target->edges[i]->innovation;
		float weight = target->edges[i]->weight;
		int outNode = target->edges[i]->out_node->nodeId;
		int inNode = target->edges[i]->in_node->nodeId;
		//this should be the most concise representation
		//seperators of 2*1 char and endl token of 1 char + 4 bytes 
		//19 bytes per edges. 
		//compressed representation of:
		//edge_count*19bytes
		fout << innov << " " << weight << " " << outNode << " " << inNode << endl;
	}
	//format separator token ~ 
	fout << "~" << endl;
	for (int i = 0; i < target->input_dimension; i++) {
		fout << target->input_nodes[i]->nodeId << endl;
	}
	fout << "~" << endl;
	for (int i = 0; i < target->output_dimension; i++) {
		fout << target->output_nodes[i]->nodeId << endl;
	}
	fout << "~" << endl;

	fout.close();
}

//NOTE: tracing split connections as in nodal neat would be
//		more efficient here.
//NOTE: this doesnt require innovation to be synced with
//		edge_count
//TODO: broken here. store is working.
network persist::load(string filename) {
	ifstream fin;
	fin.open(filename);

	network* solution = new network();
	string incoming;

	while (fin) {
		//read in the network
		//NOTE: copy constructor isnt very useful here
		getline(fin, incoming, '\n');
		if (incoming == "~") {
			break;
		}
		stringstream proc(incoming);
		int innov, outNode, inNode;
		float weight;
		//TODO: does this read in floats? it should
		proc >> innov >> weight >> outNode >> inNode;

		edge* proc_edge;
		node* proc_inNode = new node(inNode);
		node* proc_outNode = new node(outNode);

		//now parse line for values

		//check if in/outNode exist and create if not.
		bool inExists = false, outExists = false;
		for (int i = 0; i < solution->node_count; i++) {
			if (inNode == solution->nodes[i]->nodeId) {
				inExists = true;
				delete proc_inNode;
				proc_inNode = solution->nodes[i];
			}
			//not else if for loop connections
			if (outNode == solution->nodes[i]->nodeId) {
				outExists = true;
				delete proc_outNode;
				proc_outNode = solution->nodes[i];
			}
			if (outExists && inExists) {
				break;
			}
		}
		//TODO: initialize these first since throws
		//		error. this is less optimal since
		//		have to delete for every found node.
		if (!outExists) {
			//create outNode
			//proc_outNode = new node(outNode);
			force_node(solution, proc_outNode);
		}
		if (!inExists) {
			//create inNode
			//proc_inNode = new node(inNode);
			force_node(solution, proc_inNode);
		}
		//construct the edge, inNode, outNode solution
		//TODO: add_in/out_edge in node must be called since
		//		nodes exclusively contain edge references to
		//		prevent circular dependencies.
		proc_edge = new edge(proc_inNode, proc_outNode, innov, weight);
		proc_inNode->add_out_edge(proc_edge);
		proc_outNode->add_in_edge(proc_edge);
		force_edge(solution, proc_edge);
	}
	//TODO: assign input and output nodes
	//		need to signal input and output
	//		nodes in storage solution-> rely
	//		on regularities of the data structure
	//		for the file format (this should be more common
	//		throughout this project)
	//NOTE: this may change in Ancestral Speciation due to merging
	//		across domains (introducing a high nodeId as an extrema node
	//		due to introducing a higher dimension domain) prefer scalable code
	//		initially over optimal.

	//read in input nodes
	while (fin) {
		getline(fin, incoming, '\n');
		if (incoming == "~") {
			break;
		}
		stringstream proc(incoming);
		int inputNode;
		proc >> inputNode;

		//expand inputs dimension
		node** tmp = new node*[solution->input_dimension];
		for (int i = 0; i < solution->input_dimension; i++) {
			tmp[i] = solution->input_nodes[i];
		}
		delete[] solution->input_nodes;
		solution->input_dimension++;
		solution->input_nodes = new node * [solution->input_dimension];
		for (int i = 0; i < solution->input_dimension - 1; i++) {
			solution->input_nodes[i] = tmp[i];
		}
		delete[] tmp;
		//insert new input node
		for (int i = 0; i < solution->node_count; i++) {
			if (solution->nodes[i]->nodeId == inputNode) {
				solution->input_nodes[solution->input_dimension - 1] = solution->nodes[i];
			}
		}
	}

	//read in output nodes
	//TODO: repeating output node
	while (fin) {
		getline(fin, incoming, '\n');
		//TODO: doesnt happen since last but 
		//		leave for future format design
		if (incoming == "~") {
			break;
		}
		stringstream proc(incoming);
		int outputNode;
		proc >> outputNode;

		//expand output dimension
		node** tmp = new node * [solution->output_dimension];
		for (int i = 0; i < solution->output_dimension; i++) {
			tmp[i] = solution->output_nodes[i];
		}
		delete[] solution->output_nodes;
		solution->output_dimension++;
		solution->output_nodes = new node * [solution->output_dimension];
		for (int i = 0; i < solution->output_dimension - 1; i++) {
			solution->output_nodes[i] = tmp[i];
		}
		delete[] tmp;
		//insert new output node
		for (int i = 0; i < solution->node_count; i++) {
			if (solution->nodes[i]->nodeId == outputNode) {
				solution->output_nodes[solution->output_dimension - 1] = solution->nodes[i];
			}
		}
	}
	
	return solution;
}

//TODO: incrementing incorrectly results in index error. 
void persist::force_edge(network* solution, edge* proc_edge)
{
	//expand connection array
	edge** tmp = new edge * [solution->edge_count];
	for (int j = 0; j < solution->edge_count; j++) {
		tmp[j] = solution->edges[j];
	}
	delete[] solution->edges;
	solution->edge_count++;
	solution->edges = new edge * [solution->edge_count];
	for (int j = 0; j < solution->edge_count - 1; j++) {
		solution->edges[j] = tmp[j];
	}
	delete[] tmp;

	//add new edge
	solution->edges[solution->edge_count - 1] = proc_edge;
}

void persist::force_node(network* solution, node* insert_node)
{
	//expand node array
	//TODO: this is inefficient for many reasons
	node** tmp = new node * [solution->node_count + 1];
	for (int j = 0; j < solution->node_count; j++) {
		tmp[j] = solution->nodes[j];
	}
	solution->node_count++;
	delete[] solution->nodes;
	solution->nodes = new node * [solution->node_count];

	for (int j = 0; j < solution->node_count - 1; j++) {
		solution->nodes[j] = tmp[j];
	}
	delete[] tmp;
	//add the new node
	solution->nodes[solution->node_count - 1] = insert_node;
}

