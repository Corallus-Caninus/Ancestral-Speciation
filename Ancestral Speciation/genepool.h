#pragma once
#include "genome.h"
#include <random>
using namespace std;

//TODO: (post shave) coarse grained parallelization 
//		will occur here first

//TODO: THIS BE THE LAND OF LEAKS. (if leaks occur anywhere from network)
//TODO: if every class is public make as structures because c++

class genepool
{
public:
	genepool(int inputs, int outputs, mt19937& twister,
		int set_population, float set_pressure,
		float set_add_connection, float set_add_node,
		float connection_rate, float weight_rate);
	~genepool();

	int population;
	int input_dimension;
	int output_dimension;
	/// <summary>
	/// ratio of genomes to keep per generation 
	/// must be less than or equal to 1
	/// </summary>
	float pressure = 0;
	float add_connection_mutation_rate = 0;
	float add_node_mutation_rate = 0;
	float connection_mutation_rate = 0;
	float weight_mutation_rate = 0;
	genome** pool;

	//TODO: response and evaluate may be done in implementation.
	//		update is all that is truly necessary and can assert
	//		valid fitness assigned per genome. (set 0 after update
	//		and assert g.fit!=0).. has some issues as theoretically
	//		can be assigned 0.. dont assert and write good code?
	//		famous last words.
	/// <summary>
	/// create a new genepool by evaluating the fitness
	/// of current genomes and removing genomes below 
	/// pressure threshold and copying successful genomes over.
	/// </summary>
	void update(mt19937& twister);	/// <summary>

	/* done in implementation
	/// iterate float matrix by column vector through forward propagation
	/// of genepool. float matrix column span must be equal to population.
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	float** response(float**);

	/// <summary>
	/// assign the passed in array of floats with the population's
	/// fitness. 
	/// </summary>
	/// <param name=""></param>
	void evaluate(float*);
	*/
};

