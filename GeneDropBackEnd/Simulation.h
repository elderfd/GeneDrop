#pragma once
#include "Pedigree.h"
#include "Breeder.h"
#include "RNGController.h"

//! A single run of the simulation model
class Simulation {
	friend class SimulationManager;

public:
	Simulation();
	Simulation(const Pedigree& pedigree, const Breeder* breeder, const RNGController &rng);
	~Simulation();

	//! Whether or not the simulation has run and finished
	bool finished() const;

	//! Run the simulation
	void run();

protected:
	bool _finished;

	// The pedigree to be simulated
	Pedigree pedigree;

	// The object which handles the breeding of two individuals
	Breeder *breeder;

	// Each simulation needs to have its own RNG for replication
	RNGController rng;
};

