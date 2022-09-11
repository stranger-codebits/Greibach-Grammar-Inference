#ifndef POPULATION_H
#define POPULATION_H

#include "greibachchromosome.h"
#include "algovars.h"
#include <vector>

typedef std::vector<GreibachChromosome> pool;
typedef std::vector<double> pool_fitness;
typedef std::vector<double> parent_prob;
typedef std::vector<size_t> index_v;

// All the individuals are in this class
// The genetic operators are also defined here 
class Population
{
    public:
        Population();
        Population(const AlgoVars * const av_g);        // Initialize a population
        ~Population();                                  // Destructor
        
        bool next_generation(                           // Iterate once
            const DataBase & positive, 
            const DataBase & negative
        );   

        double best_fit();                              // Get best fitness
        const GreibachChromosome & best_solution();     // Get best solution

    private:
        // genetic operators
        // New offspring
        Genotype create_offspring(double s, index_v idx);
        // Crossover
        Genotype crossover(
            const GreibachChromosome & p_f,
            const GreibachChromosome & p_s
        );
        // Mutation
        Genotype mutate(
            const GreibachChromosome & p
        );
        // Calculate fitness
        void calc_fitness(
            const DataBase & positive, 
            const DataBase & negative
        );
        // members
        const AlgoVars * const av;
        pool Pool;                                      // All the individuals
        pool NextPool;                                  // Next pool of individuals
        pool_fitness PoolFitness;                       // Fitness container of all individuals
        parent_prob ParentProb;                         // Probability for a chromosome to be a parent
};

#endif