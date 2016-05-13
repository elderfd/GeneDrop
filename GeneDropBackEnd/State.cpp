#include "SimulationManager.h"
#include "State.h"


State::State() {}


State::~State() {}


void State::addOrganism(const Organism& newOrganism, std::string generationID) {
	bool added = false;

	for (auto& generation : generations) {
		if (generation.generationID == generationID) {
			generation.organisms.push_back(std::make_shared<Organism>(newOrganism));
			added = true;
			break;
		}
	}

	if (!added) {
		Generation newGeneration;
		newGeneration.generationID = generationID;
		generations.push_back(newGeneration);
		generations.back().organisms.push_back(std::make_shared<Organism>(newOrganism));
	}
	
}


unsigned int State::numberOfLoci() const {
	int number = 0;
	
	if (generations.size() > 0 && generations[0].organisms.size() > 0) {
		for (unsigned int i = 0; i < generations[0].organisms[0]->genotype().numberOfChromosomes(); i++) {
			number += generations[0].organisms[0]->genotype().chromosome(0, i).getNumberOfLoci();
		}
	}

	return number;
}


void State::seed(RNGController::SEED_TYPE seed) {
	this->generatingSeed = seed;
}


std::vector<std::shared_ptr<Organism>> State::getMatchingOrganisms(const OrganismSpecifier& specifier) const {
	std::vector<std::shared_ptr<Organism>> retVec;

	for (const auto& generation : generations) {
		OrganismSpecifier thisSpec;
		thisSpec._generation = generation.generationID;
		thisSpec.isNull = false;

		for (const auto& organism : generation.organisms) {
			thisSpec.ID = organism->name();
			
			if (thisSpec == specifier) {
				retVec.push_back(organism);
			}
		}
	}

	return retVec;
}


unsigned int State::size() const {
	unsigned int sum = 0;

	for (const auto& generation : generations) {
		sum += generation.organisms.size();
	}

	return sum;
}