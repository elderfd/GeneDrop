#include "SimulationManager.h"
#include "State.h"


State::State() {}


State::~State() {}


void State::addOrganism(const Organism& newOrganism) {
	organisms.push_back(newOrganism);
}


const std::shared_ptr<Organism> State::getOrganism(const std::string& name) const {
	std::shared_ptr<Organism> retPtr;
	
	for (auto& organism : organisms) {
		if (organism.name() == name) {
			retPtr = std::make_shared<Organism>(organism);
		}
	}

	return retPtr;
}


unsigned int State::numberOfLoci() const {
	int number = 0;
	
	if (organisms.size() > 0) {
		for (unsigned int i = 0; i < organisms[0].genotype().numberOfChromosomes(); i++) {
			number += organisms[0].genotype().chromosome(0, i).getNumberOfLoci();
		}
	}

	return number;
}


void State::seed(RNGController::SEED_TYPE seed) {
	this->generatingSeed = seed;
}