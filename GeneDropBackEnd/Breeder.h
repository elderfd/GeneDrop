#pragma once

#include "Organism.h"

class Breeder
{
public:
	Breeder();
	~Breeder();

	Organism breed(const Organism& firstParent, const Organism& secondOrganism);
};

