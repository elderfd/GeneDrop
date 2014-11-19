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

	// Now check that everything mentioned in the pedigree actually exists in the population
	std::vector<std::string> allNames = prototypePedigree.getNamesOfAllIndividuals();

	auto generateMissingOrganismError = [] (std::string name)
	{
		return "Organism designated " + name + " in the pedigree was not found anywhere in the population.";
	};

	for (auto name = allNames.begin(); name != allNames.end(); name++)
	{
		try
		{
			// Throws exception when not found
			auto organism = prototypePopulation.organismByName(*name);
		}
		catch (std::out_of_range &e)
		{
			returnValue.setValue(generateMissingOrganismError(*name));
			return returnValue;
		}
	}

	return returnValue;
}


void SimulationManager::run()
{
	// TODO: Implement
}