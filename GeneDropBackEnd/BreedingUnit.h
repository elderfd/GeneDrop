#pragma once
#include <string>
#include "Organism.h"


//! Container that provides individuals for breeding
/*!
	Depending on requirements can contain one or multiple individuals.
*/
class BreedingUnit {
public:
	BreedingUnit();
	~BreedingUnit();

	const std::string& ID() const;

protected:
	std::string _ID;
	
	std::vector<std::shared_ptr<Organism>> contents;
};

