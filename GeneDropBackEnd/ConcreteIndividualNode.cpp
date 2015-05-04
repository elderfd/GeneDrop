#include "ConcreteBreedNode.h"
#include "ConcreteIndividualNode.h"


ConcreteIndividualNode::ConcreteIndividualNode(OrganismSpecifier mother, OrganismSpecifier father) {
	breed = std::make_unique<ConcreteBreedNode>(new ConcreteBreedNode(mother, father));
}


ConcreteIndividualNode::~ConcreteIndividualNode() {}
