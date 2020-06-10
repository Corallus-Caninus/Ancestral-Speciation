#include "layer.h"
#include <iostream>
#include <assert.h>
using namespace std;

layer::layer() {
	//just so delete can bootstrap in update method
	buffer = new node*[0]; 
	buffer_size = 0;
}
layer::~layer() {
	delete[] buffer;
}

void layer::update(node** &incoming, int incoming_size) {
	//tmp gets cleaned up within method call.
	node** tmp_buffer = new node * [buffer_size];
	int filtered_incoming_size = incoming_size;
	int new_buffer_size = 0;
	bool exists = false;

	//save
	for (int i = 0; i < buffer_size; i++) {
		tmp_buffer[i] = buffer[i];
	}
	//expand
	//TODO: currently have to check twice 
	//  (once in filter and once in update 
	//	because reasons. this is faster than
	//	reallocating and should be considered 
	//	for node operations (only reallocate once
	//	per update instead of num_entries times)
	for (int i = 0; i < buffer_size; i++) {
		for (int j = 0; j < incoming_size; j++) {
			if (tmp_buffer[i] == incoming[j]) {
				filtered_incoming_size--;
				break;
			}
		}
	}
	new_buffer_size = filtered_incoming_size + buffer_size;
	delete[] buffer;
	buffer = new node*[new_buffer_size];
	//restore
	for (int i = 0; i < buffer_size; i++) {
		buffer[i] = tmp_buffer[i];
	}
	delete[] tmp_buffer;
	//update
	int g = 0; //still feels sloppy but only replace if solution is as concise
	for (int i = 0; i < incoming_size; i++) {
		for (int j = 0; j < buffer_size; j++) {
			if (buffer[j] == incoming[i]) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			//NOTE: the debugger is almost always right.
			buffer[g + buffer_size] = incoming[i];
			g++;
		}
		else {
			exists = false;
		}
	}
	buffer_size = new_buffer_size;
}

//TODOPS: this makes forward propagation even less 
//		efficient since halted nodes get a useless
//		deallocation and allocation but optimize PS.
void layer::remove(node* removal) {
	int placer = -1;
	for (int i = 0; i < buffer_size; i++) {
		//remove the entry
		if (removal == buffer[i]) {
			placer = i;
			break;
		}
	}
	assert (placer != -1); //TODOPS REMOVE ONCE STABLE

	for (int i = placer; i < buffer_size - 1; i++) {
		buffer[i] = buffer[i + 1];
	}
	node** tmp = new node * [buffer_size];
	buffer_size--;
	for (int i = 0; i < buffer_size;i++) {
		tmp[i] = buffer[i];
	}
	//resize
	delete[] buffer;
	buffer = new node * [buffer_size];
	for (int i = 0; i < buffer_size; i++) {
		buffer[i] = tmp[i];
	}
	delete[] tmp;
}

node** layer::copy_buffer() {
	node** copy = new node * [buffer_size];
	for (int i = 0; i < buffer_size; i++) {
		copy[i] = buffer[i];
	}
	return copy;
}

bool layer::final_layer(node** &check, int check_size) {
	if (check_size != buffer_size) {
		return false;
	}
	bool exists = false;
	for (int i = 0; i < check_size; i++) {
		for (int j = 0; j < buffer_size; j++) {
			if (buffer[i] == check[j]) {
				exists = true;
			}
		}
		if (!exists) {
			return false;
		}else {
			exists = true;
		}
	}
	return true;
}

void layer::reset_counters() {
	for (int i = 0; i < buffer_size; i++) {
		for (int j = 0; j < buffer[i]->num_in_edges; j++) {
			buffer[i]->in_edges[j]->recurrent_counter = 0;
		}
	}
}