#pragma once

#include <string>
#include "Genotype.h"

//! One individual in the system
/*!
	Would be able to use just genomes as individuals but might be interested in phenotypes as well later.
*/
class Organism
{
public:
	Organism();
	~Organism();

protected:
	// Identifier for this organism
	std::string name;

	Genotype genotype;
};

