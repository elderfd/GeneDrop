#include "SimulationManager.h"
#include <thread>
#include <atomic>

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
		catch (std::out_of_range)
		{
			returnValue.setValue(generateMissingOrganismError(*name));
			return returnValue;
		}
	}

	// TODO: Verify that the founder lines all have genotypes already specified

	return returnValue;
}


void SimulationManager::run()
{
	std::vector<std::thread> threads;

	// For synchronisation of multithreaded runs
	std::atomic<int> numberOfThreadsCurrentlyRunning = 0;
	int indexOfLastSimulationStarted = -1;
	
	bool keepRunning = true;

	// For passing to the thread
	auto threadFunc = [&numberOfThreadsCurrentlyRunning] (Simulation& sim)
	{
		sim.run();

		// Notify that we've finished a simulation
		numberOfThreadsCurrentlyRunning--;
	};

	while (numberOfThreadsCurrentlyRunning == 0 && indexOfLastSimulationStarted >= (int)simulations.size())
	{
		// See if we should spawn a new thread
		if (numberOfThreadsCurrentlyRunning < numberOfThreads && indexOfLastSimulationStarted < (int)simulations.size())
		{
			indexOfLastSimulationStarted++;
			threads.push_back(std::thread(threadFunc, simulations[indexOfLastSimulationStarted]));
			numberOfThreadsCurrentlyRunning++;
		}
	}
}