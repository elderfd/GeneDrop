#pragma once
#include "PedigreeNode.h"


//! A pedigree node that can be evaluated to produce new offspring
class BreedEventNode : public PedigreeNode {
public:
	BreedEventNode(PedigreeNode *firstParent, PedigreeNode *secondParent) {
		_dependencies.push_back(firstParent);
		_dependencies.push_back(secondParent);
		_evaluated = false;
		_parentsSet = true;
	}

	BreedEventNode() {
		_evaluated = false;
		_parentsSet = false;
	}

	bool complete() const {
		return _parentsSet;
	}

	void reset() {
		_evaluated = false;
	}

	// Evaluates the breed event, using the supplied breeder
	void breed(const Breeder *breeder) {
		// For now we should always have 1 or 2 parents
		assert(_dependencies.size() > 0);

		const Organism& firstParent = _dependencies[0]->organism();

		// Clear the old genotype
		result.genotype().clear();

		if (_dependencies.size() != 1) {
			const Organism& secondParent = _dependencies[1]->organism();

			breeder->breed(firstParent, secondParent, result);
		} else {
			breeder->breed(firstParent, firstParent, result);
		}

		_evaluated = true;
	}

	bool operator==(const BreedEventNode& other) const {
		return (other.result.name() == result.name());
	}

	bool operator!=(const BreedEventNode& other) const {
		return !(operator==(other));
	}

	bool evaluated() const {
		return _evaluated;
	}

	unsigned int numberOfDependencies() const {
		return _dependencies.size();
	}

	PedigreeNode* dependency(unsigned int index) {
		return _dependencies[index];
	}

	std::vector<PedigreeNode*>& dependencies() {
		return _dependencies;
	}

	const std::vector<PedigreeNode*>& dependencies() const {
		return _dependencies;
	}

	const PedigreeNode* dependency(unsigned int index) const {
		return _dependencies[index];
	}

	void setDependencies(std::vector<PedigreeNode*> dependencies) {
		this->_dependencies = dependencies;
		_parentsSet = true;
	}

protected:
	// Whether or not this event has been evaluated yet
	bool _evaluated;

	// The organism produced by this breed event
	Organism result;

	bool _parentsSet;

	// Breeding events that need to happen before this one
	std::vector<PedigreeNode*> _dependencies;

	Organism& _organism() {
		return result;
	}

	const Organism& _organism() const {
		return result;
	}
};