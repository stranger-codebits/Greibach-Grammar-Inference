#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "population.h"
#include "algovars.h"
#include "database.h"
#include "tsymbolsmap.h"

class GeneticAlgorithm
{
    public:
        GeneticAlgorithm(AlgoVars * const av_g);            // Constructor
        ~GeneticAlgorithm();                                // Destructor

        bool genetic_step(bool verbose=false);              // A step of the algorithm
        bool run_algorithm(bool verbose=false);             // Execute until convergence

    private:
        DataBase * positive_data;                           // Positive dataset
        DataBase * negative_data;                           // Negative dataset
        TSymbolsMap * algo_tsm;                             // Enumerated symbols
        Population * pool;                                  // Population

        AlgoVars * const av;                                // Algorithm variables

        GreibachChromosome *  best_chromosome;              // Best solution
        double best_fitness;                                // Best fitness

        double fitness_ceil;
        size_t step;
};

#endif