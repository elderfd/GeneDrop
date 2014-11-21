#pragma once

#include "Organism.h"

//! Handles the breeding of two individuals
class Breeder
{
public:
	Breeder();
	~Breeder();

	virtual Organism breed(const Organism& firstParent, const Organism& secondOrganism) const = 0;
};

//! Uses the Haldane mapping function for producing offspring
class HaldaneBreeder : Breeder
{
	Organism breed(const Organism& firstParent, const Organism& secondOrganism) const;
};

//! Uses the Kosambi mapping function for producing offspring
class KosambiBreeder : Breeder
{
	Organism breed(const Organism& firstParent, const Organism& secondOrganism) const;
};

