#include "SimulationManager.h"
#include <thread>
#include <atomic>

SimulationManager::SimulationManager() : breeder(&rng)
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
	auto threadFunc = [](Simulation& sim, std::atomic<int>* numberOfThreadsCurrentlyRunning)
	{
		sim.run();
	
		std::cout << "Sim finished" << std::endl;

		// Notify that we've finished a simulation
		(*numberOfThreadsCurrentlyRunning)--;
	};

	while (keepRunning)
	{
		// See if we should spawn a new thread
		if (numberOfThreadsCurrentlyRunning < numberOfThreads && indexOfLastSimulationStarted < (int)simulations.size() - 1)
		{
			indexOfLastSimulationStarted++;
			threads.push_back(std::thread(threadFunc, simulations[indexOfLastSimulationStarted], &numberOfThreadsCurrentlyRunning));
			numberOfThreadsCurrentlyRunning++;
		}

		// Check if all done
		if (indexOfLastSimulationStarted == (int)simulations.size() - 1 && numberOfThreadsCurrentlyRunning == 0)
		{
			keepRunning = false;
		}
	}

	// Have to join all threads to appease standards
	for (auto& thread : threads)
	{
		thread.join();
	}
}


void SimulationManager::generateSimulations(int numberOfSimulations)
{
	simulations.clear();

	for (int i = 0; i < numberOfSimulations; i++)
	{
		simulations.emplace_back(prototypePedigree, &breeder);
	}
}


SimulationManager::SimulationManager(const SimulationManager& other)
	: breeder(other.breeder)
{
	// Do simple copying
	this->rng = other.rng;
	this->breeder.setRNG(&this->rng);
	this->simulations = other.simulations;
	this->numberOfThreads = other.numberOfThreads;
	this->prototypePedigree = other.prototypePedigree;

	for (auto& simulation : simulations)
	{
		simulation.breeder = &this->breeder;
	}
}


SimulationManager& SimulationManager::operator=(SimulationManager other)
{
	return other;
}