#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 

#include "geneticalgorithm.h"

int main(int argc, char **argv){
	// Sample driver program of GNF inference

	// All parameters of the algorithm
	AlgoVars global_variables;
	// Parse terminal args
	parse_cmd(argc, argv, global_variables);

	// Initialize an Inference object
	GeneticAlgorithm GrammaticalInference = GeneticAlgorithm(&global_variables);

	// Calculate everything
	GrammaticalInference.run_algorithm(true);

	return 0;
}