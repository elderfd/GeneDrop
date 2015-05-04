#pragma once
#include <vector>
#include "Organism.h"
#include "OrganismSpecifier.h"


class OrganismManager {
public:
	OrganismManager();
	~OrganismManager();

	//! Returns the organism associated with the specifier
	const Organism& getOrganism(const OrganismSpecifier& specifier) const;

	//! Generates a specifier matching the specified organism
	const OrganismSpecifier generateSpecifier(const Organism& organism) const;

	//! Returns whether the listed specifier exists
	const bool contains(const OrganismSpecifier& specifier) const;

private:
	std::vector<Organism> organisms;
};

