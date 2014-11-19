#pragma once
#include "Pedigree.h"
#include "Population.h"

//! A single run of the simulation model
class Simulation
{
public:
	Simulation();
	~Simulation();

	//! Whether or not the simulation has run and finished
	bool finished() const;

protected:
	bool _finished;

	// The pedigree to be simulated
	Pedigree pedigree;

	// The population the pedigree should be applied to
	Population population;
};

