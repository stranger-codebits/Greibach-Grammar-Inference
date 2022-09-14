#include "population.h"
#include <iostream>
#include <omp.h> 
#include <vector>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::stable_sort
#include <limits.h>



// Utilities functions
template <typename T>
index_v sort_indexes(const std::vector<T> &v) {

  // initialize original index locations
  index_v idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  std::stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});

  return idx;
}

bool invalid_gnf(const AlgoVars * const av, const Genotype & g);

double calc_fitness_individual(
            const GreibachChromosome & g,
            const DataBase & positive, 
            const DataBase & negative,
            double subparse
        );

Population::Population(const AlgoVars * const av_g) : av(av_g)
{
    // Initialize a random pool
    for(size_t i = 0; i < av->P; i++) Pool.push_back(GreibachChromosome(av));
    PoolFitness = pool_fitness(av->P, 0.0);

}

Population::Population() : av(nullptr)
{
    ;
}

Population::~Population()
{
    ;
}

bool Population::next_generation(const DataBase & positive, const DataBase & negative){
    // Calculate fitness of each individual
    calc_fitness(positive, negative);

    // Sorted indexes through pool from fitness
    auto sort_idx = sort_indexes(PoolFitness);

    NextPool = pool();
    // elitism individuals
    for(size_t i = 0; i < av->e * av->P; i++){
        NextPool.push_back(Pool[sort_idx[i]]);
    }

    // Parent probabilities
    size_t max_ind = av->t * av->P;
    double fit_range = PoolFitness[sort_idx[0]] - PoolFitness[sort_idx[max_ind]];
    fit_range = (fit_range==0) ? 1 : fit_range;
    ParentProb = PoolFitness;
    double fit_sum = 0;
    for(size_t i = 0; i < max_ind; i++){
        ParentProb[sort_idx[i]] = (PoolFitness[sort_idx[i]] - PoolFitness[sort_idx[max_ind]])/fit_range;
        fit_sum += ParentProb[sort_idx[i]];
    }
    for(size_t i = max_ind; i < av->P; i++)
        ParentProb[sort_idx[i]] = 0.0;

    // crossover & mutate individuals
    for(size_t i = av->e * av->P; i < av->P;  i++){
        Genotype temp_gen = create_offspring(fit_sum, sort_idx);
        if((1.0*rand())/(1.0*RAND_MAX) < av->m) temp_gen = mutate(GreibachChromosome(temp_gen,av));
        NextPool.push_back(GreibachChromosome(temp_gen,av));
    }
    Pool = pool();
    for(size_t i = 0; i < av->P; i++) {
        Pool.push_back(NextPool[i]);
    }

    return true;
}


void Population::calc_fitness(const DataBase & positive, const DataBase & negative)
{
    PoolFitness = pool_fitness(av->P, 0.0);
#pragma omp parallel for schedule(dynamic)
    for(size_t i = 0; i < Pool.size(); i++){
        PoolFitness[i] = calc_fitness_individual(
            Pool[i], positive, negative, av->s);
    }
}


double calc_fitness_individual(
            const GreibachChromosome & g,
            const DataBase & positive, 
            const DataBase & negative,
            double subparse)
{
    double res = 0;
    for(size_t i = 0; i < positive.size(); i++){
        unsigned int f_res = g.parse(positive[i]);
        if(f_res){           // positive word not parsed
            res = res - 1.0 + subparse * float(f_res) / float(positive[i].size());
            //res = res - 1.0 + subparse * float(f_res) / float(positive[i].size())/positive.size();
        }
        else{
            res = res + 1.0; // positive word parsed
            //res = res + 1.0/positive.size();
        }
    }
    for(size_t i = 0; i < negative.size(); i++){
        unsigned int f_res = g.parse(negative[i]);
        if(f_res){
            res = res + 1.0; // negative word not parsed
            //res = res + 1.0/negative.size();
        }
        else{
            res = res - 1.0; // negative word parsed
            //res = res - 1.0/negative.size();
        }
    } 

    return res;          
}

Genotype Population::create_offspring(double s, index_v idx){
    Genotype temp_g;
    do{
        double g1 = s*(1.0*rand()/(1.0*RAND_MAX));
        double g2 = s*(1.0*rand()/(1.0*RAND_MAX));
        size_t i1, i2;
        for(size_t i = 0; i < av->P; i++){
            g1 -= ParentProb[idx[i]];
            if(g1<0){
                i1 = i;
                break;
            }
        }
        for(size_t i = 0; i < av->P; i++){
            g2 -= ParentProb[idx[i]];
            if(g2<0){
                i2 = i;
                break;
            }
        }
        if(s==0){
            i1 = rand()%Pool.size();
            i2 = rand()%Pool.size();
        }
        temp_g = crossover(Pool[idx[i1]], Pool[idx[i2]]);

    }while(invalid_gnf(av, temp_g));

    return temp_g;
}


Genotype Population::crossover(
    const GreibachChromosome & p_f,
    const GreibachChromosome & p_s
){
    // Parents
    auto g1 = p_f.get_gen();
    auto g2 = p_s.get_gen();

    // Crossover points
    size_t i1 = rand()%g1.size();
    size_t i2 = rand()%g2.size();

    // Return genotype
    auto ret_gen = Genotype(g1.begin(), g1.begin()+i1);
    ret_gen.insert( ret_gen.end(), g2.begin() + i2, g2.end() ); 
    return ret_gen;
}

Genotype Population::mutate(const GreibachChromosome & p){
    Genotype temp_g;
    do{
        temp_g = p.get_gen();
        auto idx = rand()%temp_g.size();
        temp_g[idx] = rand()%(av->max_t+av->N+1);
    }while(invalid_gnf(av, temp_g));
    return temp_g;
}

double Population::best_fit(){
    return PoolFitness[0];
}

bool invalid_gnf(const AlgoVars * const av, const Genotype & g){
    if(g.size()<2) return true;     // less than a rule
    if((g[1] > av->max_t)||(g[1]==0)) return true;
    if(g.size()>av->l) return true; // max chromosome length
    bool not_found = true;
    for(size_t i = 0; i < g.size()-1; i++){
        if(
            (g[i]>0) && (g[i] < av->max_t) &&
            (g[i+1]>0) && (g[i+1] < av->max_t)
        ){
            return true;
        }
        if(g[i]==0) not_found = false; // Starting symbol
    }
    return not_found;
}

const GreibachChromosome & Population::best_solution(){
    return Pool[0];
}