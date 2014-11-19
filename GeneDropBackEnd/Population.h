#pragma once
#include <map>
#include <string>
#include "Organism.h"
#include "Maybe.h"

//! All individuals that exist in the simulation
class Population
{
public:
	Population();
	~Population();

	// Returns the organism matching the supplied name
	Organism& organismByName(std::string name);

protected:
	// Maps the name of the organism to itself for quicker look-up
	// TODO: Maybe change this structure later
	std::map<std::string, Organism> organisms;
};

