#pragma once

//! This is the main class that sets up, runs and looks after the simulations
class SimulationManager
{
public:
	SimulationManager();
	~SimulationManager();

protected:
	//! Sets up the class into a useable state, returns false if something went wrong with set-up
	bool init();
};

