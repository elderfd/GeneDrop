#include "SimulationManager.h"
#include <thread>
#include <atomic>
#include <chrono>
#include <fstream>

#pragma warning(disable: 4996)

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


void SimulationManager::outputResultsToFile(std::string fileName)
{
	std::ofstream outFile;

	outFile.open(fileName);

	if (outFile)
	{
		// First make a header
		
		// First timestamp
		std::string timeStamp = makeTimeStamp();
		outFile << timeStamp << std::endl;

		// Then some simulation information
		outFile << "Number of loci," << prototypePedigree.founders[0]->organism().genotype().chromosome(0, 0).getNumberOfLoci() << std::endl;
		outFile << "Number of founders," << prototypePedigree.founders.size() << std::endl;
		outFile << "Number of generations," << "-" << std::endl;	//TODO: Reimplement this
		outFile << "Seed," << rng.seed() << std::endl;
		outFile << "Number of runs," << simulations.size() << std::endl;

		// Some spacer lines
		outFile << std::endl << std::endl;

		// Now header for the data
		outFile << "Run Number,ID";

		// Name all loci
		auto& prototypeGenotype = prototypePedigree.founders[0]->organism().genotype();

		for (unsigned int chromosome = 0; chromosome < prototypeGenotype.numberOfChromosomes(); chromosome++)
		{
			for (int locus = 0; locus < prototypeGenotype.chromosome(chromosome, 0).getNumberOfLoci(); locus++)
			{
				for (unsigned int copy = 0; copy < prototypeGenotype.ploidy(); copy++)
				{
					outFile << "," << prototypeGenotype.chromosome(chromosome, 0).locus(locus).getID() << "-" << copy;
				}
			}
		}

		outFile << std::endl;

		// Now actually output some data, go through simulations outputting the genotypes
		for (unsigned int simulationNumber = 0; simulationNumber < simulations.size(); simulationNumber++)
		{
			for (auto breedEvent : simulations[simulationNumber].pedigree.breedEvents)
			{
				auto& organism = breedEvent->organism();

				outFile << simulationNumber << "," << organism.name();

				for (unsigned int chromosome = 0; chromosome < prototypeGenotype.numberOfChromosomes(); chromosome++)
				{
					for (int locus = 0; locus < prototypeGenotype.chromosome(chromosome, 0).getNumberOfLoci(); locus++)
					{
						for (unsigned int copy = 0; copy < prototypeGenotype.ploidy(); copy++)
						{
							outFile << "," << prototypeGenotype.chromosome(chromosome, 0).locus(locus).getAllele();
						}
					}
				}

				outFile << std::endl;
			}
		}
	}

	outFile.close();
}


std::string SimulationManager::makeTimeStamp()
{
	auto timeNow = std::chrono::system_clock::now();
	std::time_t convertedTime = std::chrono::system_clock::to_time_t(timeNow);

	std::stringstream stampStream;
	stampStream << std::put_time(std::localtime(&convertedTime), "%H-%M-%S@%d-%m-%Y");

	return stampStream.str();
}