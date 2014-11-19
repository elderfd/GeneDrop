#pragma once

#include <string>
#include "Genotype.h"

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

