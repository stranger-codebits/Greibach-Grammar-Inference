#####################################################

cmake_minimum_required (VERSION 3.0)
project (GreibachGrammaticalInference)

#####################################################


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -fopenmp -fopt-info-vec-omp-loop")

set(EXECUTABLE_OUTPUT_PATH bin)


#####################################################

include_directories(
	${PROJECT_SOURCE_DIR}/MainProgram
	${PROJECT_SOURCE_DIR}/Utilities
	${PROJECT_SOURCE_DIR}/GeneticInference
)

#####################################################

file(COPY Datasets/ DESTINATION bin)

#####################################################

add_executable(ggi
	MainProgram/main.cpp

	Utilities/database.h
	Utilities/database.cpp
	Utilities/tsymbolsmap.h
	Utilities/tsymbolsmap.cpp
	Utilities/algovars.h

	GeneticInference/greibachchromosome.h
	GeneticInference/greibachchromosome.cpp
	GeneticInference/population.h
	GeneticInference/population.cpp
	GeneticInference/geneticalgorithm.h
	GeneticInference/geneticalgorithm.cpp
)


