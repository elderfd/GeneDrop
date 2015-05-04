#pragma once
#include <memory>
#include "BreedNode.h"
#include "OrganismSpecifier.h"


class ConcreteBreedNode : public BreedNode {
public:
	ConcreteBreedNode(std::pair<OrganismSpecifier, OrganismSpecifier> parents);
	~ConcreteBreedNode();

	bool isConcrete() const { return true; }

	std::pair<OrganismSpecifier, OrganismSpecifier> getParents() const { return chosenParents; }

private:
	std::pair<OrganismSpecifier, OrganismSpecifier> chosenParents;
};

