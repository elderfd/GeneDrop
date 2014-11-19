#pragma once

#include <string>
#include <vector>
#include "Maybe.h"
#include <iterator>

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
};

class PedigreeDepthFirstIterator
{
public:
	PedigreeDepthFirstIterator(BreedEventNode& node, int branchNumber) : branchNumber(branchNumber), _node(node)
	{};

	BreedEventNode& operator*()
	{
		return _node;
	}

	bool operator==(const PedigreeDepthFirstIterator& other)
	{
		return (other.branchNumber == branchNumber && other._node == _node);
	}

protected:
	// Needed to know which root we started from
	int branchNumber;
	BreedEventNode& _node;
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

	//! For iterating through the pedigree, depth-first of course! This is not guaranteed to provide the same individual only once (and in fact likely won't)
	PedigreeDepthFirstIterator begin();
	PedigreeDepthFirstIterator end();

	// Tests whether this pedigree structure is well-formed and usable
	Maybe<std::string> isNotUsable() const;

protected:
	// The final products of breeding
	std::vector<BreedEventNode> roots;
};

