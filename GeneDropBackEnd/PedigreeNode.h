#pragma once
#include "Organism.h"
#include "Breeder.h"


//! One node in the pedigree network, representing one breeding event between two (possibly identical) genomes
/*!
	Abstract class.
	*/
class PedigreeNode {
public:
	PedigreeNode() {}

	virtual unsigned int numberOfDependencies() const = 0;
	virtual PedigreeNode* dependency(unsigned int index) = 0;
	virtual const PedigreeNode* dependency(unsigned int index) const = 0;

	Organism& organism() {
		return _organism();
	}
	const Organism& organism() const {
		return _organism();
	}

	virtual bool evaluated() const = 0;
	virtual void breed(const Breeder *breeder) = 0;

	//! Whether or not this node is complete (i.e. has any parents if it needs them)
	virtual bool complete() const = 0;

protected:
	// Which round of breeding this breeding event is part of
	// Not important for calculation but useful for human-readable IO
	std::string generationID;	// TODO: Implement this

	virtual Organism& _organism() = 0;
	virtual const Organism& _organism() const = 0;
};