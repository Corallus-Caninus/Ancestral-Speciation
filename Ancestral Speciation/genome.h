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

//UPCOMING TEST SUITE:
//	1. merseinne twister configuration (method by reference etc.)
//  2. connection uniqueness (no parallel edges)
//  3. all combinations of operations 
//		(splitting and adding out of order)

class genome : public network
{
	genome(int inputs, int outputs, mt19937& twister);//network default
	//~genome(); UNIMPLEMENTED: inherited default

	/// <summary>
	/// add a random connection to this topology
	/// </summary>
	/// <param name="twister"></param>
	void mutate_connection(mt19937& twister);
	/// <summary>
	/// add a random node to this topology by splitting
	/// an existing node
	/// </summary>
	/// <param name="twister"></param>
	void mutate_node(mt19937& twister);

	//TODO: PS: implement last, brute force
	//			with selection pressure first
	genome crossover(genome); 
};