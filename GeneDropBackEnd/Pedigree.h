#pragma once

#include <string>
#include <vector>
#include <list>
#include "Maybe.h"
#include "BreedEventNode.h"
#include "FounderNode.h"


//! A pedigree that can simulate the breeding of the system
class Pedigree {
	friend class SimulationManagerFactory;
	friend class SimulationManager;

public:
	Pedigree();
	~Pedigree();

	Pedigree(const Pedigree& other);

	Pedigree& operator=(Pedigree other);

	//! Copies the structure of the pedigree - but not anything that's been evaluated
	// Useful because construction of these is relatively expensive
	Pedigree cloneStructureAndInitialState() const;

	//! Tests whether this pedigree structure is well-formed and usable
	Maybe<std::string> isNotUsable() const;

	//! Evaluates all of the breed events using the supplied breeder
	void evaluate(const Breeder* breeder);

	//! Adds a new organism to the pedigree
	BreedEventNode* addOrganism(std::string name);

	//! Adds a new founder organism to the pedigree
	FounderNode* addFounder(std::string name, const Genotype& genotype);

protected:
	// Used for all copy operations
	void copy(Pedigree& other) const;

	// The founders of the pedigree
	std::vector<FounderNode*> founders;

	// Everything that must be bred
	std::vector<BreedEventNode*> breedEvents;
};

