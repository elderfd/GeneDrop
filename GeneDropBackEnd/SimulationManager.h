#pragma once

#include <vector>
#include <string>
#include "Simulation.h"

//! This is the main class that sets up, runs and looks after the simulations
class SimulationManager
{
public:
	SimulationManager(std::string pedigreeFileName, std::string founderGenotypesFileName, std::string lociFileName);
	SimulationManager(std::string settingsFileName);
	~SimulationManager();

protected:
	//! Sets up the class into a useable state, returns false if something went wrong with set-up
	bool init();

	//! The simulations that the manager is looking after
	std::vector<Simulation> simulations;
};

