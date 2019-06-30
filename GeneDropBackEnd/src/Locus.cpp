#include "Locus.h"
#include <cassert>

Locus::Locus(std::string ID, double geneticDistance, std::string allele) {
	this->ID = ID;
	this->geneticDistance = geneticDistance;
	this->allele = allele;
}

Locus::Locus() {
	ID = "defaultID";
	geneticDistance = 0;
	allele = "0";
}

std::string Locus::getAllele() const {
	return allele;
}

double Locus::getGeneticDistance() const {
	return geneticDistance;
}

std::string Locus::getID() const {
	return ID;
}

void Locus::setAllele(std::string allele) {
	this->allele = allele;
}

void Locus::setGeneticDistance(double newGeneticDistance) {
	geneticDistance = newGeneticDistance;
}

void Locus::setID(std::string ID) {
	this->ID = ID;
}

bool Locus::operator==(const Locus &other) const {
	assert(other.ID == ID);

	if (other.allele == allele) {
		return true;
	}

	return false;
}

bool Locus::operator!=(const Locus &other) const {
	assert(other.ID == ID);

	return !(operator==(other));
}

bool Locus::operator<(const Locus &other) const {
	assert(other.ID == ID);

	if (allele < other.allele) {
		return true;
	}

	return false;
}

