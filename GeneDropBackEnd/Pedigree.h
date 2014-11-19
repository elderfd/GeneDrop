#pragma once

#include <string>
#include <vector>
#include "Maybe.h"

//! One node in the pedigree network, representing one breeding event between two (possibly identical) genomes
class BreedEventNode
{
public:
	BreedEventNode(std::string firstParent, std::string secondParent);

	// Parents involved in this breeding event
	std::string firstParent;
	std::string secondParent;

	// Breeding events that need to happen before this one
	std::vector<BreedEventNode&> dependencies;

	// The name of the offspring produced by this event 
	std::string name;

	// Whether or not this event has been passed through before (for detecting cycles)
	bool covered;

	// Which round of breeding this breeding event is part of
	// Not important for calculation but useful for human-readable IO
	std::string roundID;
};

//! Network representation of a pedigree
class Pedigree
{
public:
	Pedigree();
	~Pedigree();

	//! Copies the structure of the pedigree - but not anything that's been evaluated
	// Useful because construction of these is relatively expensive
	Pedigree clone();

protected:
	// Tests whether this pedigree structure is well-formed and usable
	Maybe<std::string> isUsable() const;

	// The final products of breeding
	std::vector<BreedEventNode> roots;
};

