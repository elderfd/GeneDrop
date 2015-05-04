#include "ConcreteBreedNode.h"


ConcreteBreedNode::ConcreteBreedNode(std::pair<OrganismSpecifier, OrganismSpecifier> parents) {
	chosenParents = parents;
}


ConcreteBreedNode::~ConcreteBreedNode() {}
