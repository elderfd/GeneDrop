#include "SimulationManager.h"


SimulationManager::SimulationManager()
{
}


SimulationManager::~SimulationManager()
{
}


Maybe<std::string> SimulationManager::verifySimulationPrototype()
{
	Maybe<std::string> returnValue;

	// Check pedigree by itself first
	if (auto error = prototypePedigree.isNotUsable())
	{
		return error;
	}

	
}