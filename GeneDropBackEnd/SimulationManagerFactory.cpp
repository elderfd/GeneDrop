#include "SimulationManagerFactory.h"
#include <fstream>


SimulationManagerFactory::SimulationManagerFactory()
{
}


SimulationManagerFactory::~SimulationManagerFactory()
{
}


SimulationManager SimulationManagerFactory::createFromSimpleInput(std::string pedigreeFileName, std::string genotypeFileName, std::string lociFileName)
{
	// TODO: Implement

	// Start with the pedigree file
	std::ifstream pedigreeFile;
	pedigreeFile.open(pedigreeFileName);



	pedigreeFile.close();

	return SimulationManager();
}