#pragma once

#include <vector>
#include <string>
#include "Simulation.h"
#include "RNGController.h"
#include "Maybe.h"

//! This is the main class that sets up, runs and looks after the simulations
class SimulationManager
{
	friend class SimulationManagerFactory;

public:
	SimulationManager();
	~SimulationManager();

	SimulationManager(const SimulationManager& other);

	//! Runs the simulations
	void run();

	//! Checks that the prototypes can all work together
	Maybe<std::string> verifySimulationPrototype();

	//! Builds the specified number of simulations
	void generateSimulations(int numberOfSimulations);

protected:
	//! Handles all of the random number generation
	RNGController rng;

	//! The simulations that the manager is looking after
	std::vector<Simulation> simulations;

	// For now assume that all simulations will use same data, relatively easy to change later
	Pedigree prototypePedigree;

	// How many processes to spawn to run simulations
	int numberOfThreads;

	// For now we only have the Haldane mapping function
	// TODO: Implement choice of breeders
	HaldaneBreeder breeder;

};


