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
	SimulationManager createFromSimpleInput(std::string pedigreeFileName, std::string genotypeFileName, std::string lociFileName);

protected:
	// Main file parsing functions
	void parsePedigreeFile(std::string fileName);
	void parseLociFile(std::string fileName);
	void parseGenotypeFile(std::string fileName);


};