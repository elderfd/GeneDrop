#include "Cross.h"


Cross::Cross() {}


Cross::Cross(const OrganismSpecifier& father, const OrganismSpecifier& mother, const OrganismSpecifier& child) {
	this->father = father;
	this->mother = mother;
	this->child = child;
}


Cross::~Cross() {}
