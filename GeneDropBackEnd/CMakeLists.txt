add_library(Backend STATIC
	src/Breeder.h
    src/Breeder.cpp
    src/Chromosome.h
	src/Chromosome.cpp
    src/Cross.h
	src/Cross.cpp
    src/Genotype.h
	src/Genotype.cpp
    src/Locus.h
	src/Locus.cpp
    src/Organism.h
	src/Organism.cpp
    src/OrganismSpecifier.h
	src/OrganismSpecifier.cpp
    src/OutputMaker.h
	src/OutputMaker.cpp
    src/Pedigree.h
	src/Pedigree.cpp
    src/RNGController.h
	src/RNGController.cpp
    src/SimulationManager.h
	src/SimulationManager.cpp
    src/State.h
	src/State.cpp
	src/TimeHandler.h
	src/TimeHandler.cpp
)

target_include_directories(Backend PUBLIC src)
