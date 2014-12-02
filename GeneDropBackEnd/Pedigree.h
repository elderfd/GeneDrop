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
	std::string roundID;

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
		dependencies.push_back(firstParent);
		dependencies.push_back(secondParent);
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

	// Evaluates the breed event, using the supplied breeder
	void breed(const Breeder *breeder)
	{
		// For now we should always have 1 or 2 parents
		assert(dependencies.size() > 0);

		const Organism& firstParent = dependencies[0]->organism();

		if (dependencies.size() != 1)
		{
			const Organism& secondParent = dependencies[1]->organism();

			breeder->breed(firstParent, secondParent, result);
		}
		else
		{
			breeder->breed(firstParent, firstParent, result);
		}
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

	void setDependencies(std::vector<PedigreeNode*> dependences)
	{
		this->dependencies = dependences;
	}

protected:
	// Whether or not this event has been evaluated yet
	bool _evaluated;

	// The organism produced by this breed event
	Organism result;

	bool _parentsSet;

	// Breeding events that need to happen before this one
	std::vector<PedigreeNode*> dependencies;

	Organism& _organism()
	{
		assert(_evaluated);

		return result;
	}

	const Organism& _organism() const
	{
		assert(_evaluated);

		return result;
	}
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
	Pedigree cloneStructureAndInitialState();

	//! Tests whether this pedigree structure is well-formed and usable
	Maybe<std::string> isNotUsable() const;

	//! Returns the names of every individual mentioned
	std::vector<std::string> getNamesOfAllIndividuals();

	//! Evaluates all of the breed events using the supplied breeder
	void evaluate(const Breeder* breeder);

	//! Adds a new organism to the pedigree
	void addOrganism(std::string name, std::string firstParentName, std::string secondParentName);

	//! Adds a new founder organism to the pedigree
	void addFounder(std::string name, const Genotype& genotype);

protected:
	//! Returns the node with the specified name, or a false if not found
	Maybe<PedigreeNode*> findNodeByName(std::string name);

	// The founders of the pedigree
	std::vector<FounderNode> roots;

	// The final products of breeding - will never point to founders
	std::vector<BreedEventNode*> leaves;

	// Flat vector of all nodes in pedigree that have been produced
	std::vector<PedigreeNode*> nodes;
};

