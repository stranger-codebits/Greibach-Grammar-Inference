# Greibach-Grammar-Inference
Grammatical Inference of Greibach Normal Form context-free grammars with genetic programming using positive and negative examples.

This is a project in development. The goal of this project is to 
create context-free grammars from datasets using genetic algorithms.
The conventions used are that each grammar representation follows 
the [Greibach Normal Form](https://en.wikipedia.org/wiki/Greibach_normal_form)
formalism. Goal is from a file containing words that should belong
to a context-free grammar (positive examples) and a file that contains data that should not belong
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
-N Define the size of the non-Terminals set (Default 10)
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

Typical experiments results are written in this csv format

|Dataset|generations|generations|generations|non-terminals|non-terminals|non-terminals|rules|rules|rules|gen_len|gen_len|gen_len|accuracy|accuracy          |accuracy|TPR     |TPR               |TPR     |TNR     |TNR               |TNR |mean_time          |
|-------|-----------|-----------|-----------|-------------|-------------|-------------|-----|-----|-----|-------|-------|-------|--------|------------------|--------|--------|------------------|--------|--------|------------------|----|-------------------|
|       |min        |mean       |max        |min          |mean         |max          |min  |mean |max  |min    |mean   |max    |min     |mean              |max     |min     |mean              |max     |min     |mean              |max |                   |
|L1     |11         |18.8       |37         |9            |9.7          |10           |18   |25.3 |29   |63     |86.1   |98     |1.0     |1.0               |1.0     |1.0     |1.0               |1.0     |1.0     |1.0               |1.0 |1.1502296630991622 |
|L2     |500        |500.0      |500        |4            |7.6          |9            |16   |24.9 |32   |45     |77.2   |99     |0.959016|0.9639342         |0.97541 |0.863636|0.8863633999999999|1.0     |0.97    |0.9810000000000001|0.99|49.95902102509863  |
|L3     |4          |9.4        |20         |10           |10.0         |10           |22   |26.6 |32   |64     |89.5   |100    |1.0     |1.0               |1.0     |1.0     |1.0               |1.0     |1.0     |1.0               |1.0 |0.35658437489764766|
|L4     |500        |500.0      |500        |7            |8.2          |10           |22   |28.1 |35   |59     |78.3   |93     |0.954955|0.9747747999999999|0.990991|0.821429|0.9071428000000001|1.0     |0.975904|0.9975904         |1.0 |57.85853273650282  |
|L5     |500        |500.0      |500        |4            |7.8          |10           |18   |24.4 |30   |56     |74.1   |94     |0.858268|0.88189           |0.96063 |0.333333|0.4629627         |1.0     |0.95    |0.9949999999999999|1.0 |259.06548082319784 |
|L6     |500        |500.0      |500        |7            |8.5          |10           |28   |31.4 |36   |81     |95.2   |100    |0.915254|0.9364407         |0.957627|0.444444|0.6111111         |0.833333|0.96    |0.9949999999999999|1.0 |39.894286070304226 |
|L7     |10         |31.6       |53         |9            |9.5          |10           |19   |26.3 |34   |57     |83.0   |98     |1.0     |1.0               |1.0     |1.0     |1.0               |1.0     |1.0     |1.0               |1.0 |1.1154056298022623 |
|L8     |75         |377.2      |500        |8            |9.1          |10           |19   |29.9 |36   |63     |86.0   |100    |0.987179|0.9955128999999999|1.0     |1.0     |1.0               |1.0     |0.98    |0.993             |1.0 |110.78919129450223 |

TPR -> True Positive rate
TNR -> True Negative rate

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

## Work Citation 
If you use any of this work please cite this [paper](https://www.mdpi.com/2624-6120/3/4/42)

Anastasopoulos, N.; Dermatas, E. Building Greibach Normal Form Grammars Using Genetic Algorithms. Signals 2022, 3, 708-720. https://doi.org/10.3390/signals3040042 

