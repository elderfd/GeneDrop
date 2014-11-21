#pragma once

#include <string>
#include <vector>
#include "Maybe.h"
#include <iterator>
#include <functional>
#include "Breeder.h"
#include <cassert>
#include <list>

//! One node in the pedigree network, representing one breeding event between two (possibly identical) genomes
/*!
	A breeding event produces a single organism.
*/
class BreedEventNode
{
public:
	BreedEventNode(Organism *firstParent, Organism *secondParent)
	{
		this->firstParent = firstParent;
		this->secondParent = secondParent;
		_evaluated = false;
	}

	BreedEventNode(const Organism &organism)
	{
		_result = organism;
		_evaluated = true;
	}

	// Evaluates the breed event, using the supplied breeder
	void breed(const Breeder *breeder)
	{
		breeder->breed(*firstParent, *secondParent, _result);
	}

	bool operator==(const BreedEventNode& other) const
	{
		return (other._result.name() == _result.name());
	}

	bool operator!=(const BreedEventNode& other) const
	{
		return !(operator==(other));
	}

	bool evaluated()
	{
		return _evaluated;
	}

	Organism& result()
	{
		assert(_evaluated);

		return _result;
	}

	const Organism& result() const
	{
		assert(_evaluated);

		return _result;
	}

	unsigned int numberOfDependencies() const
	{
		return dependencies.size();
	}

	BreedEventNode* dependency(unsigned int index)
	{
		return dependencies[index];
	}

	const BreedEventNode* dependency(unsigned int index) const
	{
		return dependencies[index];
	}

protected:
	// Parents involved in this breeding event
	const Organism* firstParent;
	const Organism* secondParent;

	// Whether or not this event has been evaluated yet
	bool _evaluated;

	// The organism produced by this breed event
	Organism _result;

	// Which round of breeding this breeding event is part of
	// Not important for calculation but useful for human-readable IO
	std::string roundID;

	// Breeding events that need to happen before this one
	std::vector<BreedEventNode*> dependencies;
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

	//! Evaluates all of the breed events
	void evaluate(const Breeder* breeder);

	//! Returns the node with the specified name, or a false if not found
	Maybe<BreedEventNode*> findNodeByName(std::string name);

protected:
	// The final products of breeding
	std::list<BreedEventNode> leaves;

	// The founders of the pedigree
	std::list<BreedEventNode> roots;
};

