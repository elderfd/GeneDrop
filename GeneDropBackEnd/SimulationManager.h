#pragma once

#include <vector>
#include <string>
#include "Simulation.h"
#include "RNGController.h"

//! This is the main class that sets up, runs and looks after the simulations
class SimulationManager
{
	friend class SimulationManagerFactory;

public:
	SimulationManager();
	~SimulationManager();

protected:
	//! Sets up the class into a useable state, returns false if something went wrong with set-up
	bool init();

	//! Handles all of the random number generation
	RNGController rng;

	//! The simulations that the manager is looking after
	std::vector<Simulation> simulations;
};


