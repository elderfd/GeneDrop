#pragma once
#include <utility>
#include "ParentSupplier.h"


class ConcreteBreedNode;


//! Node connecting children to their parents
class BreedNode {
public:
	BreedNode();
	~BreedNode();

	//! Produces a concrete version of this node
	std::unique_ptr<ConcreteBreedNode> concretise() const;

	virtual bool isConcrete() const { return false; }

	virtual std::pair<OrganismSpecifier, OrganismSpecifier> getParents() const = 0;
};


class CrossBreedNode : public BreedNode {
public:
	std::pair<OrganismSpecifier, OrganismSpecifier> getParents() const {
		return std::make_pair(mother.get(), father.get());
	}

private:
	ParentSupplier mother;
	ParentSupplier father;
};


class SelfBreedNode : public BreedNode {
	std::pair<OrganismSpecifier, OrganismSpecifier> getParents() const {
		OrganismSpecifier chosenParent = parent.get();

		return std::make_pair(chosenParent, chosenParent);
	}

private:
	ParentSupplier parent;
};