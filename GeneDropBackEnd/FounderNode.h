#pragma once
#include <string>
#include "PedigreeNode.h"


//! A pedigree node that has no parents and cannot be evaluated. Is always at the root of the tree.
class FounderNode : public PedigreeNode {
public:
	FounderNode() {}

	FounderNode(std::string name, const Genotype& genotype) {
		founder.setName(name);
		founder.setGenotype(genotype);
	}

	unsigned int numberOfDependencies() const {
		return 0;
	}

	PedigreeNode* dependency(unsigned int index) {
		assert(false);
		return nullptr;
	}

	const PedigreeNode* dependency(unsigned int index) const {
		assert(false);
		return nullptr;
	}

	bool evaluated() const {
		return true;
	}

	void breed(const Breeder *breeder) {
		assert(false);
	}

	bool complete() const {
		return true;
	}

protected:
	Organism founder;

	Organism& _organism() {
		return founder;
	}

	const Organism& _organism() const {
		return founder;
	}
};