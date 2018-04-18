set(LIB_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${Configuration} PARENT_SCOPE)

add_library(Backend STATIC
	Breeder.h
    Breeder.cpp
    Chromosome.h
	Chromosome.cpp
    Cross.h
	Cross.cpp
    Genotype.h
	Genotype.cpp
    Locus.h
	Locus.cpp
    Organism.h
	Organism.cpp
    OrganismSpecifier.h
	OrganismSpecifier.cpp
    OutputMaker.h
	OutputMaker.cpp
    Pedigree.h
	Pedigree.cpp
    RNGController.h
	RNGController.cpp
    SimulationManager.h
	SimulationManager.cpp
    State.h
	State.cpp
	TimeHandler.h
	TimeHandler.cpp
)

target_include_directories(Backend PUBLIC include)
