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