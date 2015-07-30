#pragma once
#include <iostream>
#include <memory>
#include "Organism.h"
#include "RNGController.h"
#include <string>
#include <vector>

class OutputMaker;

class State {
	friend class OutputMaker;

public:
	State();
	~State();

	void addOrganism(const Organism& newOrganism);

	const std::shared_ptr<Organism> getOrganism(const std::string& name) const;

	friend OutputMaker& operator<<(OutputMaker &out, State &state);

	unsigned int numberOfLoci() const;

	unsigned int size() const { return organisms.size(); }

	// TODO: Fix this
	const Genotype& getPrototypeGenotype() const {
		return organisms[0].genotype();
	}

	void seed(RNGController::SEED_TYPE seed);

private:
	std::vector<Organism> organisms;
	RNGController::SEED_TYPE generatingSeed = 0;
};

