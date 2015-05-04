#pragma once
#include "IndividualNode.h"
#include "Organism.h"
#include "OrganismSpecifier.h"


class ConcreteIndividualNode : public IndividualNode {
public:
	ConcreteIndividualNode(std::unique_ptr<ConcreteBreedNode> breedNode);
	~ConcreteIndividualNode();

	const bool isRealised() const { return true; }
	const bool isFounder() const { return _isFounder; }

private:
	Organism realisation;

	//! Whether or not this if a founder node
	bool _isFounder = false;
};

