#pragma once
#include "OrganismSpecifier.h"
#include <string>


class Cross {
public:
	Cross();
	Cross(const OrganismSpecifier& father, const OrganismSpecifier& mother, const OrganismSpecifier& child);
	~Cross();

	OrganismSpecifier father;
	OrganismSpecifier mother;
	OrganismSpecifier child;
};

