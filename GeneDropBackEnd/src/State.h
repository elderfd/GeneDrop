#pragma once
#include <iostream>
#include <memory>
#include "Organism.h"
#include "RNGController.h"
#include <string>
#include <vector>

class OrganismSpecifier;
class OutputMaker;

class State {
	friend class OutputMaker;

public:
	State();
	~State();

	void addOrganism(const Organism& newOrganism, std::string generationID);

	std::vector<std::shared_ptr<Organism>> getMatchingOrganisms(const OrganismSpecifier& specifier) const;

	friend OutputMaker& operator<<(OutputMaker &out, State &state);

	size_t numberOfLoci() const;

	size_t size() const;

	// TODO: Fix this
	const Genotype& getPrototypeGenotype() const {
		return generations[0].organisms[0]->genotype();
	}

	void seed(RNGController::SEED_TYPE seed);

private:
	class Generation {
	public:
		std::string generationID;
		std::vector<std::shared_ptr<Organism>> organisms;
	};

	std::vector<Generation> generations;
	RNGController::SEED_TYPE generatingSeed = 0;
};

