#pragma once
#include <memory>


class BreedNode;
class ConcreteIndividualNode;


class IndividualNode {
public:
	IndividualNode();
	~IndividualNode();

	virtual const bool isRealised() const { return false; }
	virtual const bool isFounder() const { return breed == nullptr; }

	//! Builds a concrete version of this node
	std::unique_ptr<ConcreteIndividualNode> concretise() const;

protected:
	//! The node that can joins this node to its parent nodes
	std::unique_ptr<BreedNode> breed = nullptr;
};