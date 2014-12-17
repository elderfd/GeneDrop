#pragma once

#include <string>
#include <vector>
#include "Maybe.h"
#include <iterator>
#include <functional>
#include "Breeder.h"
#include <cassert>
#include <list>
#include <deque>

class Pedigree;

//! One node in the pedigree network, representing one breeding event between two (possibly identical) genomes
/*!
	Abstract class.
*/
class PedigreeNode
{
public:
	PedigreeNode()
	{}

	virtual unsigned int numberOfDependencies() const = 0;
	virtual PedigreeNode* dependency(unsigned int index) = 0;
	virtual const PedigreeNode* dependency(unsigned int index) const = 0;

	Organism& organism()
	{
		return _organism();
	}
	const Organism& organism() const
	{
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


//! A pedigree node that has no parents and cannot be evaluated. Is always at the root of the tree.
class FounderNode : public PedigreeNode
{
public:
	FounderNode()
	{}

	FounderNode(std::string name, const Genotype& genotype)
	{
		founder.setName(name);
		founder.setGenotype(genotype);
	}

	unsigned int numberOfDependencies() const
	{
		return 0;
	}

	PedigreeNode* dependency(unsigned int index)
	{
		assert(false);
		return nullptr;
	}

	const PedigreeNode* dependency(unsigned int index) const
	{
		assert(false);
		return nullptr;
	}

	bool evaluated() const
	{
		return true;
	}

	void breed(const Breeder *breeder)
	{
		assert(false);
	}

	bool complete() const
	{
		return true;
	}

protected:
	Organism founder;

	Organism& _organism()
	{
		return founder;
	}

	const Organism& _organism() const
	{
		return founder;
	}
};


//! A pedigree node that can be evaluated to produce new offspring
class BreedEventNode : public PedigreeNode
{
public:
	BreedEventNode(PedigreeNode *firstParent, PedigreeNode *secondParent)
	{
		_dependencies.push_back(firstParent);
		_dependencies.push_back(secondParent);
		_evaluated = false;
		_parentsSet = true;
	}

	BreedEventNode()
	{
		_evaluated = false;
		_parentsSet = false;
	}

	bool complete() const
	{
		return _parentsSet;
	}

	void reset()
	{
		_evaluated = false;
	}

	// Evaluates the breed event, using the supplied breeder
	void breed(const Breeder *breeder)
	{
		// For now we should always have 1 or 2 parents
		assert(_dependencies.size() > 0);

		const Organism& firstParent = _dependencies[0]->organism();

		// Clear the old genotype
		result.genotype().clear();

		if (_dependencies.size() != 1)
		{
			const Organism& secondParent = _dependencies[1]->organism();

			breeder->breed(firstParent, secondParent, result);
		}
		else
		{
			breeder->breed(firstParent, firstParent, result);
		}

		_evaluated = true;
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

	unsigned int numberOfDependencies() const
	{
		return _dependencies.size();
	}

	PedigreeNode* dependency(unsigned int index)
	{
		return _dependencies[index];
	}

	std::vector<PedigreeNode*>& dependencies()
	{
		return _dependencies;
	}

	const std::vector<PedigreeNode*>& dependencies() const
	{
		return _dependencies;
	}

	const PedigreeNode* dependency(unsigned int index) const
	{
		return _dependencies[index];
	}

	void setDependencies(std::vector<PedigreeNode*> dependencies)
	{
		this->_dependencies = dependencies;
		_parentsSet = true;
	}

protected:
	// Whether or not this event has been evaluated yet
	bool _evaluated;

	// The organism produced by this breed event
	Organism result;

	bool _parentsSet;

	// Breeding events that need to happen before this one
	std::vector<PedigreeNode*> _dependencies;

	Organism& _organism()
	{
		return result;
	}

	const Organism& _organism() const
	{
		return result;
	}
};

//! A pedigree that can simulate the breeding of the system
class Pedigree
{
	friend class SimulationManagerFactory;
	friend class SimulationManager;

public:
	Pedigree();
	~Pedigree();

	Pedigree(const Pedigree& other);

	Pedigree& operator=(Pedigree other);

	//! Copies the structure of the pedigree - but not anything that's been evaluated
	// Useful because construction of these is relatively expensive
	Pedigree cloneStructureAndInitialState() const;

	//! Tests whether this pedigree structure is well-formed and usable
	Maybe<std::string> isNotUsable() const;

	//! Evaluates all of the breed events using the supplied breeder
	void evaluate(const Breeder* breeder);

	//! Adds a new organism to the pedigree
	BreedEventNode* addOrganism(std::string name);

	//! Adds a new founder organism to the pedigree
	FounderNode* addFounder(std::string name, const Genotype& genotype);

protected:
	// Used for all copy operations
	void copy(Pedigree& other) const;

	// The founders of the pedigree
	std::vector<FounderNode*> founders;

	// Everything that must be bred
	std::vector<BreedEventNode*> breedEvents;
};

