#include "BreedNode.h"
#include "ConcreteBreedNode.h"


BreedNode::BreedNode() {}


BreedNode::~BreedNode() {}


std::unique_ptr<ConcreteBreedNode> BreedNode::concretise() const {
	auto concrete = std::make_unique<ConcreteBreedNode>(new ConcreteBreedNode(this->getParents()));

	return concrete;
}