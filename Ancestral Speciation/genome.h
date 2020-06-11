#pragma once
#include <random>
#include "network.h"
using namespace std;

// NOTE: it would be best to inherit network and implement
//		 innovation etc here to allow Ancestral Speciations' RoM
//		 to inherit network topology. 
//	this is post shave:
//	currently, network is fundamentally implemented with
//	NEAT operations. still implement inheritance in case
//	a rework is desired  and therein as a practice in 
//	sustainable development

// NOTE: genome and network must be fed a twister
//		 since would take up too much space in the object.
//		 also pointer isnt associated with object to allow changes
//		 to twister and to lessen allocation headaches.
// NOTE: only temporary/evaluation locals should occur here. network
//		 will be used for serialization genome is a runtime only object.

class genome 
	//: public network
{
public:
	//TODO: (post-shave why not) twister should be passed 
	//		in ONCE and retrieved from super-class
	genome(int inputs, int outputs, mt19937& twister);
		//:network(inputs, outputs, twister) {};
		//:network(
		//inputs, outputs, twister); //network default
	~genome(); //TODO: (post shave) inhereted default

	network* net;
	float fitness;

	/// <summary>
	/// add a random connection to this topology
	/// </summary>
	/// <param name="twister"></param>
	bool mutate_connection(mt19937& twister);
	/// <summary>
	/// add a random node to this topology by splitting
	/// an existing node
	/// </summary>
	/// <param name="twister"></param>
	void mutate_node(mt19937& twister);

	//TODO: PS: implement last, brute force
	//			with selection pressure first
	//			and test the manure out of it.
	//genome crossover(genome); 
};
