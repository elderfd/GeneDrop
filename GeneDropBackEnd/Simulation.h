#pragma once
#include "Pedigree.h"
#include "Breeder.h"
#include <memory>

//! A single run of the simulation model
class Simulation
{
public:
	Simulation();
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
};

