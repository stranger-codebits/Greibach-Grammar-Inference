# Greibach-Grammar-Inference
Grammatical Inference of Greibach Normal Form context-free grammars with genetic programming using positive and negative examples.

This is a project in development. The goal of this project is to 
create context-free grammars from datasets using genetic algorithms.
The conventions used are that each grammar representation follows 
the [Greibach Normal Form](https://en.wikipedia.org/wiki/Greibach_normal_form)
formalism. Goal is from a file containing words that should belong
to a context-free grammar (psitive examples) and a file that contains data that should not belong
in the same context-free grammar (negative examples), to generate the corresponding CFG.

## Getting Started
This project is completely developed in C/C++ in linux environments.
For execution in any other OS any PRs are welcome. 
The C++ compiler used is GCC with openmp capabilities for multithreading.

There is a small script to execute experiments in batch that is written 
in Python3 and also needs Pandas.

### Installing
This project is cmake based. In order to build the code execute:

```
git clone https://github.com/stranger-codebits/Greibach-Grammar-Inference.git
cd Greibach-Grammatical-Inference
cmake .
make
```

In the bin directory there is an executable named ggi.
For instructions on usage, type

```
./ggi -h
```

## Algorithm description

### General idea
This project aims at using genetic operators in order to derive
a Context-Free grammar from positive and negative examples.

More descriptions to be added.

### Algorithm options

```
-h Print the help screen
-p Define the positive dataset
-n Define the negative dataset
-g Define the maximum number of generations (Default 500)
-m Define the mutation rate (Default 0.03)
-P Define the size of the population (Default 500)
-f Define the convergence fitness (0.0 ~ 1.0). (Default 1.0)
         if bestIndividualfitness > f * maxEstimatedFitness, then algorithm halts.
-N Define the size of the non-Terminals set (Default 0)
-t Define the rate of individuals that can be selected as parents (Default 0.85)
-e Define the elitism rate (Default 0.03)
-r Define the maximum rule length in the initial population (Default 5)
-R Define the maximum number of rules in the initial population (Default 40, at least 2)
-l Define the max genome length for all gens (Default Max rule length * Max number of rules)
-s Define the subparse variable (Default 0.01)
-o Define the output file
-i Define the random seed
```

You must at least define at least the arguments ``` -p "Path/To/Positive" -n "Path/To/Negative" ```
or the programm will not initiate the genetic algorithm.

Note that the majority of the arguments have been tested under normal circumstances. If somebody finds
a strange value like ```-N 0```, then you may fall into undefined behaviour. Check beforehands that each 
argument has a valid value or do not mess with predefined values.

### Dataset convention

The datasets are single text files (e.g. one for positive and one for negative data)

There is no need to define the terminals set as these are inferred during file reading.

You can find more examples on the datasets folder. The datasets are created according to
this [paper](https://www.researchgate.net/publication/311866701_Inductive_Synthesis_of_Cover-Grammars_with_the_Help_of_Ant_Colony_Optimization)

### Testing/Experimenting
In order to test the effectiveness of the method you can try to execute this script:

```
python3 RunExperiments.py
```

## Work In Progress

There are several things in active development.

### Active Development
- Accelerate the executable
- Integrate the software in a different library

### Next steps
- Readable representation of derived grammars
- Rule reduction wherever possible
- Stricter validity chacks for genetic operators

## Additional information

1. This software was written in vscode, many thanks to the community
2. Contact the author of this software [here](https://www.linkedin.com/in/anastasopoulosnikolas/)
3. Definetely feel free to share any improvements for this software by opening a PR

## Licensing
This project is licensed under the GNU GPL v3.0 license.

