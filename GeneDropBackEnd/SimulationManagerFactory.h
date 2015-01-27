#pragma once

#include "SimulationManager.h"
#include <string>

//! Produces an appropriate SimulationManager from input data
class SimulationManagerFactory
{
public:
	SimulationManagerFactory();
	~SimulationManagerFactory();

	//! Produces a SimulationManager matching the specification in pedigree, loci and genotype files
	static SimulationManager createFromSimpleInput(std::string pedigreeFileName, std::string genotypeFileName, std::string lociFileName, int numberOfRuns, int numberOfThreads);

protected:


};