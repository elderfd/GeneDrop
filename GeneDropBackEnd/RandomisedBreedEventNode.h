#pragma once
#include "BreedEventNode.h"
#include "OrganismChoicePool.h"


class RandomisedBreedEventNode : public BreedEventNode {
public:
	RandomisedBreedEventNode();
	RandomisedBreedEventNode(OrganismChoicePool* selfingPool);
	RandomisedBreedEventNode(OrganismChoicePool* firstParentPool, OrganismChoicePool* secondParentPool);
	~RandomisedBreedEventNode();
};

