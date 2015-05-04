#include "BreedNode.h"
#include "ConcreteIndividualNode.h"
#include "IndividualNode.h"


IndividualNode::IndividualNode() {}


IndividualNode::~IndividualNode() {}


std::unique_ptr<ConcreteIndividualNode> IndividualNode::concretise() const {
	auto concrete = std::make_unique<ConcreteIndividualNode>(new ConcreteIndividualNode(breed->concretise()));

	return concrete;
}