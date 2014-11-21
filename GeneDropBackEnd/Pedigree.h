#pragma once

#include <string>
#include <vector>
#include "Maybe.h"
#include <iterator>
#include <functional>
#include "Breeder.h"

//! One node in the pedigree network, representing one breeding event between two (possibly identical) genomes
class BreedEventNode
{
public:
	BreedEventNode(std::string firstParent, std::string secondParent)
	{
		this->firstParent = firstParent;
		this->secondParent = secondParent;
	}
	BreedEventNode(){}

	// Breeding events that need to happen before this one
	std::vector<BreedEventNode*> dependencies;

	// The name of the offspring produced by this event 
	std::string name;

	// The organism produced by this breed event
	Organism result;

	// Evaluates the breed event, using the supplied breeder
	void breed(const Breeder *breeder);

	bool operator==(const BreedEventNode& other)
	{
		return (other.name == name);
	}

	bool operator!=(const BreedEventNode& other)
	{
		return !(operator==(other));
	}

	// Which round of breeding this breeding event is part of
	// Not important for calculation but useful for human-readable IO
	std::string roundID;

	// Whether or not this event has been evaluated yet
	bool evaluated;

	// Parents involved in this breeding event
	std::string firstParent;
	std::string secondParent;
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

	//! Tests whether this pedigree structure is well-formed and usable
	Maybe<std::string> isNotUsable() const;

	//! Returns the names of every individual mentioned
	std::vector<std::string> getNamesOfAllIndividuals();

protected:
	// The final products of breeding
	std::vector<BreedEventNode> roots;
};

