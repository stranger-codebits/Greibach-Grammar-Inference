#ifndef ALGOVARS_H
#define ALGOVARS_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>

// Here are all the algorithm variables and default values
// This is like a singleton across all files
struct AlgoVars
{
    // Options
    std::string p;              // Path to positive data
    std::string n;              // Path to negative data
    size_t g = 500;             // Number of generations
    float m = 0.03;             // Mutation rate
    size_t P = 500;             // Population size
    float f = 1.0;              // Convergence fitness rate
    size_t N = 10;              // Non-terminals set size
    float t = 0.85;             // Percentage of parents
    float e = 0.03;             // Elitism rate
    size_t r = 5;               // Maximum rule length in initialization
    size_t R = 20;              // Maximum number of rules in initial population
    size_t l = 100;             // Max genotype length
    float s = 0.01;             // Subparse variable
    std::string o;              // Output file
    unsigned int seed = 0;      // Random seed  

    // Infered vars
    unsigned int max_t = 1;     // Max terminal symbol 
};

void print_usage();
inline bool checkfile(const char * const name);

inline void parse_cmd(int argc, char **argv, AlgoVars &av){
    const char *short_options="hp:n:g:m:P:f:N:t:r:R:l:s:c:o:i:e:";
    int next_option;
    while ((next_option  = getopt(argc, argv, short_options)) != -1){
        switch(next_option){
            case 'h':
                std::cout << "~ HELP MENU ~" << std::endl;
                print_usage();
                exit(1);
                break;
            case 'p':
                if(checkfile(optarg)) av.p = std::string(optarg);
                else{
                    std::cout << "Could not open: " << optarg << " exiting..." << std::endl;
                    exit(1); 
                }
                break;
            case 'n':
                if(checkfile(optarg)) av.n = std::string(optarg);
                else{
                    std::cout << "Could not open: " << optarg << " exiting..." << std::endl;
                    exit(1); 
                }
                break;
            case 'g':
                av.g = atoi(optarg);
                break;
            case 'm':
                av.m = atof(optarg);
                if(av.m>1){
                    std::cout << "Mutation rate can not exceed 1, exiting..." << std::endl;
                    exit(1);
                }
                break;
            case 'P':
                av.P = atoi(optarg)
            case 'f':
                av.f = atof(optarg);
                if((av.f>1)||(av.f<0)){
                    std::cout << "Convergence rate must be in [0, 1], exiting..." << std::endl;
                    exit(1);
                }
                break;
            case 'N':
                av.N = atoi(optarg);
                break;
            case 't':
                av.t = atof(optarg);
                if((av.t>1)||(av.t<=0)){
                    std::cout << "Parental rate must be in (0, 1], exiting..." << std::endl;
                    exit(1);
                }
                break;
            case 'e':
                av.e = atof(optarg);
                if((av.e>=1)||(av.t<=0)){
                    std::cout << "Elitism rate must be in (0, 1), exiting..." << std::endl;
                    exit(1);
                }
                break;
            case 'r':
                av.r = atoi(optarg);
                if(av.r < 2){
                    std::cout << "Rule length must be >= 2, exiting..." << std::endl;
                    exit(1);
                }
                break;
            case 'R':
                av.R = atoi(optarg);
                if(av.R < 2){
                    std::cout << "Grammar rules must be >= 2, exiting..." << std::endl;
                    exit(1);
                }
                break;
            case 'l':
                av.l = atoi(optarg);
                if(av.l < 2){
                    std::cout << "Grammar length must be >= 2, exiting..." << std::endl;
                    exit(1);
                }
                break;
            case 's':
                av.s = atof(optarg);
                break;
            case 'o':
                av.o = std::string(optarg);
                break;
            case 'i':
                av.seed = atoi(optarg);
                break;
            default:
                std::cout << "Ignoring unknown argument..." << std::endl;
                break;
        }
    }
}

inline void print_usage()
{
    std::cout << "\t-h Print the help screen" << "\n" <<
                 "\t-p Define the positive dataset" << "\n" <<
                 "\t-n Define the negative dataset" << "\n" <<
                 "\t-g Define the maximum number of generations (Default 500)" << "\n" <<
                 "\t-m Define the mutation rate (Default 0.03)" << "\n" <<
                 "\t-P Define the size of the population (Default 500)" << "\n" <<
                 "\t-f Define the convergence fitness (0.0 ~ 1.0). (Default 1.0) \n\t if bestIndividualfitness > f * maxEstimatedFitness, then algorithm halts." << "\n" <<
                 "\t-N Define the size of the non-Terminals set (Default 10)" << "\n" <<
                 "\t-t Define the rate of individuals that can be selected as parents (Default 0.85)" << "\n" <<
                 "\t-e Define the elitism rate (Default 0.03)" << "\n" <<
                 "\t-r Define the maximum rule length in the initial population (Default 5)" << "\n" <<
                 "\t-R Define the maximum number of rules in the initial population (Default 40, at least 2)" << "\n" <<
                 "\t-l Define the max genome length for all gens (Default Max rule length * Max number of rules)" << "\n" <<
                 "\t-s Define the subparse variable (Default 0.01)" << "\n" <<
                 "\t-o Define the output file" << "\n" <<
                 "\t-i Define the random seed\n";
    std::cout << " >> Least arguments are ./ggi -p \"Positive.txt\" -n \"Negative.txt\" " << std::endl;
}

inline bool checkfile(const char * const name) {
    std::ifstream f(name);
    return f.good();
}
#endif //ALGOVARS_H
