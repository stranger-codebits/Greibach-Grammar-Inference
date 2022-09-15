#include "greibachchromosome.h"
#include <iostream>
#include <cmath>

typedef unsigned int Symbol;
typedef std::vector<Symbol> NTArray;

// Parser utilities
bool inline ParseWord(
    Symbol StartSymbol, const Grammar & G, const EnWord &w, // args
    size_t start_pointer, size_t end_pointer,               // In order to not create new vectors - optimization
    bool & Parsed, unsigned int & Depth                     // Return by reference
);

bool inline Distribute(
    NTArray & non_terms, const Grammar & G, const EnWord & w,   // args
    size_t start_pointer, size_t end_pointer,                   // optimization
    bool & Parsed, unsigned int & Depth                         // Return by reference
);


// Some general usability functions
inline unsigned int _random_terminal(const AlgoVars * const av){
    return rand()%(av->max_t) + 1;
}

inline unsigned int _random_non_terminal(const AlgoVars * const av){
    unsigned int sym = rand()%(av->N);
    if(sym) return sym + av->max_t;
    else return sym;    
}

inline Rule _generate_random_rule(const AlgoVars * const av){
    // at least len 2 for a rule length
    size_t rule_len = rand()%(av->r - 1) + 2;
    Rule r;

    // Head
    r.push_back(_random_non_terminal(av));
    // Terminal in first position of body
    r.push_back(_random_terminal(av));

    // Rest of rule body
    for(size_t i = 0; i < rule_len - 2; i++){
        r.push_back(_random_non_terminal(av));
    }

    return r;
}



GreibachChromosome::GreibachChromosome(const AlgoVars * const av_g)
: av(av_g)
{
    // Number of rules
    RuleNum = rand()%(size_t(std::floor(av->R / 2.0))) + size_t(std::ceil(av->R/2.0));

    // Create the rules
    for(size_t i = 0; i < RuleNum; i++){
        auto temp_rule = _generate_random_rule(av);
        Phen.push_back(temp_rule);
        Gen.insert(Gen.end(), temp_rule.begin(), temp_rule.end());
    }

    
}

// Chromosome from genotype
GreibachChromosome::GreibachChromosome(Genotype g, const AlgoVars * const av_g) : av(av_g)
{
    Gen = g;
    size_t prev_start=0;
    for(size_t i = 0; i < Gen.size()-2; i++){
        if((Gen[i+2]>0)&&(Gen[i+2]<=av->max_t)){
            Phen.push_back(
                Rule(Gen.begin()+prev_start, Gen.begin()+i+1)
            );
            prev_start = i+1;
        }
    }
    Phen.push_back(
        Rule(Gen.begin()+prev_start, Gen.end())
    );
    RuleNum = Phen.size();
}

GreibachChromosome::GreibachChromosome() : av(nullptr)
{
    ;
}

GreibachChromosome::~GreibachChromosome(){
    ;
}


const Grammar & GreibachChromosome::get_grammar() const{
    return Phen;
}

const Genotype & GreibachChromosome::get_gen() const{
    return Gen;
}

unsigned int GreibachChromosome::parse(const EnWord &w) const
{
    unsigned int d = 0;
    bool parsed = false;
    ParseWord(0, Phen, w, 0, w.size(), parsed, d);
    return d;
}

// Parser utilities
bool inline ParseWord(
    Symbol StartSymbol, const Grammar & G, const EnWord &w, // args
    size_t start_pointer, size_t end_pointer,               // In order to not create new vectors - optimization
    bool & Parsed, unsigned int & Depth                     // Return by reference
){
    // What to return if no rule matches
    unsigned int minimum_depth = end_pointer - start_pointer;
    unsigned int w_size = end_pointer - start_pointer;
    Depth = minimum_depth;

    // Try to match
    for(Rule r : G){
        if((r[0]==StartSymbol) && (r[1]==w[start_pointer]) && (w_size>=r.size()-1)){ // Rule matched
            // Case 1: 1 Terminal, 1 Symbol
            if((w_size==1)&&(r.size()==2)){
                Parsed = true;
                Depth = 0;
                return true;
            }
            // Case 1.5: No match but a sub-string parsed
            else if((w_size>1)&&(r.size()==2)){
                if(minimum_depth > Depth - 1){
                    minimum_depth = Depth - 1;
                }
            }
            // Case 2: One non-terminal in body of rule
            else if((w_size>1)&&(r.size()==3)){
                unsigned int _minimum_depth = minimum_depth - 1;
                ParseWord(r[2], G, w, start_pointer+1, end_pointer, Parsed, _minimum_depth);
                minimum_depth = (_minimum_depth < minimum_depth) ? 
                                _minimum_depth : minimum_depth;
                if(Parsed){
                    Depth = 0;
                    return true;
                }
            }
            // Case 3: multiple NT Symbols on body
            else if((w.size()>1)&&(r.size()>3)){
                unsigned int _minimum_depth = minimum_depth - 1;
                NTArray nts(r.begin()+2, r.end());
                Distribute(nts, G, w, start_pointer+1, end_pointer, Parsed, _minimum_depth);
                minimum_depth = (_minimum_depth < minimum_depth) ? 
                                _minimum_depth : minimum_depth;
                if(Parsed){
                    Depth = 0;
                    return true;
                }
            }
        }
        else if((r[0]==StartSymbol) && (r[1]==w[start_pointer]) && (w_size<r.size()-1) && (minimum_depth > 1)){
            minimum_depth = minimum_depth - 1;
        }
    }
    Depth = minimum_depth;
    return false;
}

bool inline Distribute(
    NTArray & non_terms, const Grammar & G, const EnWord & w,   // args
    size_t start_pointer, size_t end_pointer,                   // optimization
    bool & Parsed, unsigned int & Depth                         // Return by reference
){
    unsigned int minimum_depth = end_pointer - start_pointer;
    Depth = minimum_depth;
    bool b1=false,b2=false;
    unsigned int d1, d2, w_size;
    d1 = d2 = w_size = minimum_depth;

    if(non_terms.size()==2){ // Case 1: 2 Terminals
        for(size_t i = start_pointer + 1; i < end_pointer; i++){
            ParseWord(non_terms[0], G, w, start_pointer, i, b1, d1);
            ParseWord(non_terms[1], G, w, i, end_pointer, b2, d2);
            minimum_depth = (d1+d2) < minimum_depth ? (d1+d2) : minimum_depth;
            if(b1&&b2){
                Depth = 0;
                Parsed = true;
                return true;
            }
        }
    } 
    else{ // Case 2: many terminals
        for(size_t i = start_pointer + 1; i < end_pointer - non_terms.size() + 1; i++){
            NTArray new_nts(non_terms.begin()+1, non_terms.end());
            ParseWord(non_terms[0], G, w, start_pointer, i, b1, d1);
            Distribute(new_nts, G, w, i, end_pointer, b2, d2);
            minimum_depth = (d1+d2) < minimum_depth ? (d1+d2-1) : minimum_depth;
            if(b1&&b2){
                Depth = 0;
                Parsed = true;
                return true;
            }
        }
    }
    Depth = minimum_depth;
    Parsed = false;
    return false;
}