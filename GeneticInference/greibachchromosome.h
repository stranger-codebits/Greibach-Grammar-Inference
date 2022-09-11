#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include "algovars.h"
#include "database.h"


// Each chromosome encapsulates all the GNF functionality

typedef std::vector<unsigned int> Genotype;
typedef std::vector<unsigned int> Rule;
typedef std::vector<Rule> Grammar;

class GreibachChromosome
{
    public:
        GreibachChromosome(const AlgoVars * const av_g);    // Random chromosome
        GreibachChromosome(                                 // Chromosome from genotype
            Genotype g,
            const AlgoVars * const av_g
        );
        GreibachChromosome();             
        ~GreibachChromosome();                              // Destructor

        const Grammar & get_grammar() const;                // Get the grammar
        const Genotype & get_gen() const;                   // Get the chromosome

        unsigned int parse(const EnWord &w) const;          // Parse the data word amd 
                                                            // return the # of missed chars
    
    private:
        Genotype Gen;
        Grammar Phen;
        const AlgoVars * const av;
        size_t RuleNum;
};

#endif // CHROMOSOME_H