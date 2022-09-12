#include "geneticalgorithm.h"
#include <iostream>
#include <unistd.h>
#include <set>

typedef std::vector<float> out_stat;
//Accuracy, TPR, TNR
out_stat res_stats(const GreibachChromosome &g, DataBase &p, DataBase &n, const AlgoVars * const av);
//Terminals, Rules, Chromosome length
out_stat gnf_stats(const GreibachChromosome &g, const AlgoVars * const av);

GeneticAlgorithm::GeneticAlgorithm(AlgoVars * const av_g) 
: av(av_g), step(0)
{
    std::cout << "~~~ Initialization ~~~" << std::endl;
    std::cout << "[1] Reading data: " << std::endl;
    std::cout << "\t";
    positive_data = new DataBase(av->p);
    std::cout << "\t";
    negative_data = new DataBase(av->n);

    std::cout << "[2] Counting terminal symbols: ";
    algo_tsm = new TSymbolsMap();
    algo_tsm->ParseData(*positive_data);
	algo_tsm->ParseData(*negative_data);
    std::cout << algo_tsm->max_c() << std::endl;

    std::cout << "[3] Enumerating datasets: " << std::endl;
    std::cout << "\t - Positive... ";
    positive_data->enumerate_db(*algo_tsm);
    std::cout << "Done" << std::endl;
    std::cout << "\t - Negative... ";
	negative_data->enumerate_db(*algo_tsm);
    std::cout << "Done" << std::endl;

    std::cout << "[4] Initializing population: ";
    av->max_t = algo_tsm->max_c();
    pool = new Population(av); 
    std::cout << av->P << std::endl;

    fitness_ceil = positive_data->size() + negative_data->size();
    std::cout << "[5] Max fitness: " << fitness_ceil << std::endl;

    std::cout << "[6] Seed: " << av->seed << std::endl;
    srand(av->seed);

    std::cout << "~~~ Initialization Done ~~~" << std::endl;
}


GeneticAlgorithm::~GeneticAlgorithm()
{
    std::cout << "~~~ Termination ~~~" << std::endl;

    std::cout << "[1] Writing results... " << std::endl;
    auto sol = pool->best_solution();
    // Step, P, V, Accuracy, TPR, TNR, Accuracy
    if(av->o.size()){
        std::ofstream outfile;
        outfile.open(av->o, std::ios_base::app);
        outfile << step << ", ";
        for (auto _r : gnf_stats(sol, av)) outfile << _r << ", ";
        for (auto _r : res_stats(sol, *positive_data, *negative_data, av))
            outfile << _r << ", ";
        
        outfile << std::endl;
    }
    else{
	for (auto _r : gnf_stats(sol, av)) std::cout << _r << ", ";
	for (auto _r : res_stats(sol, *positive_data, *negative_data, av))
		            std::cout << _r << ", ";
	std::cout << std::endl;
    }

    std::cout << "[2] Deleting population... ";
    delete pool;
    std::cout << "Done" << std::endl; 

    std::cout << "[3] Deleting symbols map... ";
    delete algo_tsm;
    std::cout << "Done" << std::endl; 

    std::cout << "[4] Deleting datasets:" << std::endl;
    std::cout << "\t - Negative... ";
    delete negative_data;
    std::cout << "Done" << std::endl; 
    std::cout << "\t - Positive... ";
    delete positive_data;
    std::cout << "Done" << std::endl; 

    std::cout << "~~~ Termination Done ~~~" << std::endl;
}


bool GeneticAlgorithm::genetic_step(bool verbose){
    step = step+1;
    pool->next_generation(*positive_data, *negative_data);
    if(verbose){
        std::cout << "[Step: " << step << "] : " << pool->best_fit() << std::endl;
    }
    return true;
}

bool GeneticAlgorithm::run_algorithm(bool verbose){
    bool exit_criterion = false;
    do{
        genetic_step(verbose);
        exit_criterion = (step >= av->g) || (pool->best_fit() >= av->f * fitness_ceil);
    }while(!exit_criterion);
    return true;
}


//Accuracy, TPR, TNR
out_stat res_stats(const GreibachChromosome &g, DataBase &p, DataBase &n, const AlgoVars * const av){
    float acc = 0;
    float tpr = 0;
    float tnr = 0;
    // Positive data
    for(size_t i = 0; i < p.size(); i++){
        auto _pos = p[i];
        if(g.parse(_pos)==0){
            acc++;
            tpr++;
        }
    }
    // Negative data
    for(size_t i = 0; i < n.size(); i++){
        auto _neg = n[i];
        if(g.parse(_neg)!=0){
            acc++;
            tnr++;
        }
    }
    acc = acc/(p.size()+n.size());
    tpr = tpr/p.size();
    tnr = tnr/n.size();
    return out_stat({acc, tpr, tnr});
}

//Non-Terminals, Rules, Chromosome length
out_stat gnf_stats(const GreibachChromosome &g, const AlgoVars * const av){
    std::set<unsigned int> s;
    for(auto sym : g.get_gen()){
        if(sym>av->max_t) s.insert(sym);
        else s.insert(0);
    }
    return out_stat({(float) s.size(), (float) g.get_grammar().size(), (float) g.get_gen().size()});
}
