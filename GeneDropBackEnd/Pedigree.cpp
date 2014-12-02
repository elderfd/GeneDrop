#include "Pedigree.h"
#include <functional>
#include "Maybe.h"
#include <map>

Pedigree::Pedigree()
{
}


Pedigree::~Pedigree()
{
	// TODO: Delete everything
}


Maybe<std::string> Pedigree::isNotUsable() const
{
	// Depth-first recursion and make sure that all dependencies can be resolved
	//	Cycles in pedigree are unsolvable
	//	No breed event should have the same name

	// TODO: Second check might also guarantee the first in this implementation

	auto returnValue = Maybe<std::string>();

	//auto generateCycleErrorMessage = [] (const BreedEventNode *firstNode, const BreedEventNode *secondNode)
	//{
	//	return "Cycle discovered in pedigree (i.e. an organism is its own ancestor). Found back edge between " + firstNode->organism().name() + " and " + secondNode->organism().name() + ".";
	//};

	//auto generateDuplicateErrorMessage = [](const BreedEventNode &node)
	//{
	//	return "Organism designated as " + node.organism().name() + " was produced more than once in the pedigree.";
	//};

	//std::map<std::string, const BreedEventNode*> namesAndBreedEventsFound;

	//std::function<Maybe<std::string> (const BreedEventNode*)> checkDependencies = [&] (const BreedEventNode *node)
	//{
	//	Maybe<std::string> returnValue;

	//	// Check that we haven't seen this name produced by a different breed event elsewhere
	//	try
	//	{
	//		if (*namesAndBreedEventsFound.at(node->organism().name()) != *node)
	//		{
	//			returnValue.setValue(generateDuplicateErrorMessage(*node));
	//		}
	//	}
	//	catch (std::out_of_range)
	//	{
	//		// Not in map so far so add it
	//		namesAndBreedEventsFound[node->organism().name()] = node;
	//	}

	//	for (unsigned int i = 0; i < node->numberOfDependencies(); i++)
	//	{
	//		// Check for cycle by looking for back edge
	//		try
	//		{
	//			namesAndBreedEventsFound.at(node->dependency(i)->organism().name());
	//		}
	//		catch (std::out_of_range)
	//		{
	//			// If cycle found, produce an error
	//			returnValue.setValue(generateCycleErrorMessage(node, node->dependency(i)));
	//			return returnValue;
	//		}

	//		returnValue = checkDependencies(node->dependency(i));
	//	}

	//	return returnValue;
	//};

	//Maybe<std::string> returnValue;

	//for (auto it = leaves.begin(); it != leaves.end(); it++)
	//{
	//	if (auto error = checkDependencies(&(*it)))
	//	{
	//		returnValue.setValue(error.value());
	//	}
	//}

	return returnValue;
}


std::vector<std::string> Pedigree::getNamesOfAllIndividuals()
{
	std::vector<std::string> returnVec;
	std::map<std::string, bool> foundSoFar;

	std::function<void (PedigreeNode*)> addNode = [&] (PedigreeNode* node)
	{
		try
		{
			foundSoFar.at(node->organism().name());
		}
		catch (std::out_of_range)
		{
			// If not found then add it
			returnVec.push_back(node->organism().name());
			foundSoFar[node->organism().name()] = true;
		}

		for (unsigned int i = 0; i < node->numberOfDependencies(); i++)
		{
			addNode(node->dependency(i));
		}
	};

	for (auto it = leaves.begin(); it != leaves.end(); it++)
	{
		addNode((*it));
	}

	return returnVec;
}


void Pedigree::evaluate(const Breeder* breeder)
{
	std::function<void (PedigreeNode*, const Breeder*)> recursiveEvaluate = [&](PedigreeNode *node, const Breeder* breeder)
	{
		// Evaluate all dependencies, then evaluate yourself
		for (unsigned int i = 0; i < node->numberOfDependencies(); i++)
		{
			recursiveEvaluate(node->dependency(i), breeder);
		}

		if (!node->evaluated())
		{
			node->breed(breeder);
		}
	};

	for (auto it = leaves.begin(); it != leaves.end(); it++)
	{
		recursiveEvaluate(*it, breeder);
	}
}


Maybe<PedigreeNode*> Pedigree::findNodeByName(std::string name)
{
	Maybe<PedigreeNode*> retVal;

	std::function<bool(PedigreeNode*, Maybe<PedigreeNode*>&)> findFunc = [&](PedigreeNode* node, Maybe<PedigreeNode*> &found)
	{
		if (node->organism().name() == name)
		{
			retVal.setValue(node);
			return true;
		};

		// Check all dependencies
		for (unsigned int i = 0; i < node->numberOfDependencies(); i++)
		{
			if (findFunc(node->dependency(i), found))
			{
				retVal.setValue(node);
				return true;
			}
		}

		return false;
	};

	for (auto it = roots.begin(); it != roots.end(); it++)
	{
		if (findFunc(&(*it), retVal))
		{
			break;
		}
	}

	return retVal;
}


void Pedigree::addOrganism(std::string name, std::string firstParentName, std::string secondParentName)
{
	// Get the parents
	Maybe<PedigreeNode*> firstParent = findNodeByName(firstParentName);
	Maybe<PedigreeNode*> secondParent = findNodeByName(secondParentName);

	// TODO: Remove repetition here

	// If parents don't exist then make them
	if (!firstParent)
	{
		nodes.push_back(new BreedEventNode);
		nodes.back()->organism().setName(firstParentName);
		firstParent.setValue(nodes.back());
	}
	else
	{
		// Check whether this parent is currently a leaf (and remove it if so)
		for (auto it = leaves.begin(); it != leaves.end(); it++)
		{
			if (*it == firstParent.value())
			{
				leaves.erase(it);
				break;
			}
		}
	}

	if (!secondParent)
	{
		nodes.push_back(new BreedEventNode);
		nodes.back()->organism().setName(secondParentName);
		secondParent.setValue(nodes.back());
	}
	else
	{
		// Check whether this parent is currently a leaf (and remove it if so)
		for (auto it = leaves.begin(); it != leaves.end(); it++)
		{
			if (*it == secondParent.value())
			{
				leaves.erase(it);
				break;
			}
		}
	}

	// Make the child
	BreedEventNode* childNode = new BreedEventNode(firstParent.value(), secondParent.value());
	childNode->organism().setName(name);
	
	// Add to our list o things
	nodes.push_back(childNode);
}


void Pedigree::addFounder(std::string name, const Genotype& genotype)
{
	roots.emplace_back(name, genotype);

	// Add to the list of things we've added
	nodes.push_back(&roots.back());
}