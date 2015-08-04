#include "OrganismSpecifier.h"


OrganismSpecifier::OrganismSpecifier() {}


OrganismSpecifier::~OrganismSpecifier() {}


OrganismSpecifier::OrganismSpecifier(std::string ID, std::string generation) {
	this->ID = ID;
	this->_generation = generation;
	this->isNull = false;
}


bool OrganismSpecifier::operator==(const OrganismSpecifier& other) const {
	// Nulls are never the same
	if (this->isNull || other.isNull) return false;

	// Weak equality on generation
	bool generationIsSame = this->_generation.empty() || other._generation.empty() || this->_generation == other._generation;
	
	// Weak equality on name (for selecting whole generations)
	bool IDIsSame = this->ID.empty() || other.ID.empty() || this->ID == other.ID;

	return generationIsSame && IDIsSame;
}


OrganismSpecifier OrganismSpecifier::fromString(std::string inString) {
	const std::string delim = "::";
	auto delimIndex = inString.find(delim);

	OrganismSpecifier specifier;

	if (delimIndex == std::string::npos) {
		specifier.ID = inString;
	} else {
		specifier.ID = inString.substr(delimIndex + 2);
		specifier._generation = inString.substr(0, delimIndex);
	}

	specifier.isNull = false;

	return specifier;
}