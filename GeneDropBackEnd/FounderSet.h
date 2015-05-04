#pragma once
#include <memory>
#include <vector>
#include "BreedingUnit.h"
#include "Organism.h"


class FounderSet {
public:
	FounderSet();
	~FounderSet();

	void addFounder(std::shared_ptr<Organism> founder, std::string unitIdentifier);

private:
	std::vector<BreedingUnit> units;
};

