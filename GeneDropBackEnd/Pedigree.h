#pragma once

#include <string>
#include <vector>
#include "Maybe.h"
#include <iterator>
#include <functional>

//! One node in the pedigree network, representing one breeding event between two (possibly identical) genomes
class BreedEventNode
{
public:
	BreedEventNode(std::string firstParent, std::string secondParent);

	// Parents involved in this breeding event
	std::string firstParent;
	std::string secondParent;

	// Breeding events that need to happen before this one
	std::vector<BreedEventNode*> dependencies;

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

//class PedigreeDepthFirstIterator
//{
//public:
//	PedigreeDepthFirstIterator(BreedEventNode& node) : _node(node)
//	{};
//
//	BreedEventNode& operator*()
//	{
//		return _node;
//	}
//
//	bool operator==(const PedigreeDepthFirstIterator& other)
//	{
//		return (nodeStackAndBranchTaken == other.nodeStackAndBranchTaken);
//	}
//
//	bool operator!=(const PedigreeDepthFirstIterator& other)
//	{
//		return !(operator==(other));
//	}
//
//	bool operator++()
//	{
//		BreedEventNode* desiredNode = nullptr;
//
//		std::function<Maybe<PedigreeDepthFirstIterator>> findNext = [&] (PedigreeDepthFirstIterator& it)
//		{
//			// If we have dependencies then return the first of those
//			if (it._node.dependencies.size() > 0)
//			{
//				it.nodeStackAndBranchTaken.emplace_back(it._node, 0);
//				it._node = it._node.dependencies[0];
//
//				return it;
//			}
//			else // Move back up the stack
//			{
//
//			}
//		};
//
//		// If we have no dependencies then move up stack
//		if (_node.dependencies.size() == 0)
//		{
//			nodeStackAndBranchTaken.pop_back();
//		}
//
//		auto topOfStack = std::get<0>(nodeStackAndBranchTaken.back());
//		auto lastBranch = std::get<1>(nodeStackAndBranchTaken.back());
//
//		// If last dependency then pop
//		if (lastBranch == (topOfStack.dependencies.size() - 1))
//		{
//			
//		}
//	}
//
//	bool invalid()
//	{
//		return _invalid;
//	}
//
//protected:
//	// Need to know which path we took to get where we are now
//	std::vector<std::pair<BreedEventNode&, int>> nodeStackAndBranchTaken;
//	BreedEventNode& _node;
//	bool _invalid;
//};

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
	//PedigreeDepthFirstIterator begin()
	//{
	//	PedigreeDepthFirstIterator it;

	//	if ()
	//}

	//PedigreeDepthFirstIterator end();

	//! Tests whether this pedigree structure is well-formed and usable
	Maybe<std::string> isNotUsable() const;

	//! Returns the names of every individual mentioned
	std::vector<std::string> getNamesOfAllIndividuals();

protected:
	// The final products of breeding
	std::vector<BreedEventNode> roots;
};

