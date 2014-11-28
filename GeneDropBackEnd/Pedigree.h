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
	Abstract class.
*/
class PedigreeNode
{
public:
	virtual unsigned int numberOfDependencies() const = 0;
	virtual PedigreeNode* dependency(unsigned int index) = 0;
	virtual const PedigreeNode* dependency(unsigned int index) const = 0;

	virtual Organism& organism() = 0;
	virtual const Organism& organism() const = 0;

	virtual bool evaluated() const = 0;
	virtual void breed(const Breeder *breeder) = 0;

	virtual std::pair<const Organism*, const Organism*> parents() = 0;

protected:
	// Which round of breeding this breeding event is part of
	// Not important for calculation but useful for human-readable IO
	std::string roundID;
};


//! A pedigree node that has no parents and cannot be evaluated. Is always at the root of the tree.
class FounderNode : public PedigreeNode
{
public:
	FounderNode()
	{}

	unsigned int numberOfDependencies() const
	{
		return 0;
	}

	PedigreeNode* dependency(unsigned int index)
	{
		assert(false);
		return nullptr;
	}

	std::pair<const Organism*, const Organism*> parents()
	{
		return std::pair<const Organism*, const Organism*>(nullptr, nullptr);
	}

	const PedigreeNode* dependency(unsigned int index) const
	{
		assert(false);
		return nullptr;
	}

	Organism& organism()
	{
		return founder;
	}

	const Organism& organism() const
	{
		return founder;
	}

	bool evaluated() const
	{
		return true;
	}

	void breed(const Breeder *breeder)
	{
		assert(false);
	}

protected:
	Organism founder;
};


//! A pedigree node that can be evaluated to produce new offspring
class BreedEventNode : public PedigreeNode
{
public:
	BreedEventNode(Organism *firstParent, Organism *secondParent)
	{
		this->firstParent = firstParent;
		this->secondParent = secondParent;
		_evaluated = false;
	}

	//BreedEventNode(const Organism &organism)
	//{
	//	_result = organism;
	//	_evaluated = true;
	//}

	std::pair<const Organism*, const Organism*> parents()
	{
		return std::pair<const Organism*, const Organism*>(firstParent, secondParent);
	}

	// Evaluates the breed event, using the supplied breeder
	void breed(const Breeder *breeder)
	{
		breeder->breed(*firstParent, *secondParent, result);
	}

	bool operator==(const BreedEventNode& other) const
	{
		return (other.result.name() == result.name());
	}

	bool operator!=(const BreedEventNode& other) const
	{
		return !(operator==(other));
	}

	bool evaluated() const
	{
		return _evaluated;
	}

	Organism& organism()
	{
		assert(_evaluated);

		return result;
	}

	const Organism& organism() const
	{
		assert(_evaluated);

		return result;
	}

	unsigned int numberOfDependencies() const
	{
		return dependencies.size();
	}

	PedigreeNode* dependency(unsigned int index)
	{
		return dependencies[index];
	}

	const PedigreeNode* dependency(unsigned int index) const
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
	Organism result;

	// Breeding events that need to happen before this one
	std::vector<BreedEventNode*> dependencies;
};

//! Network representation of a pedigree
class Pedigree
{
	friend class SimulationManagerFactory;

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
	Maybe<PedigreeNode*> findNodeByName(std::string name);

protected:
	// The final products of breeding
	std::list<BreedEventNode> leaves;

	// The founders of the pedigree
	std::list<FounderNode> roots;
};

