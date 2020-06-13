#pragma once
#include "genome.h"
#include <random>
using namespace std;
// TODO: manage genomes and evaluation 
//		 (no crummy evaluator from last time)

//TODO: (post shave) coarse grained parallelization 
//		will occur here first

//TODO: THIS BE THE LAND OF LEAKS. (if leaks occur anywhere from network)

class genepool
{
	genepool(int inputs, int outputs, mt19937 &twister);
	~genepool();

	float pressure = 0;

	/// <summary>
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

	/// <summary>
	/// create a new genepool by evaluating the fitness
	/// of current genomes and removing genomes below 
	/// pressure threshold and copying successful genomes over.
	/// </summary>
	void update();
};

