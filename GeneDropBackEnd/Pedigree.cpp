#include "Pedigree.h"
#include <functional>
#include "Maybe.h"
#include <map>

Pedigree::Pedigree()
{
}


Pedigree::~Pedigree()
{
}


Maybe<std::string> Pedigree::isNotUsable() const
{
	// Depth-first recursion and make sure that all dependencies can be resolved
	//	Cycles in pedigree are unsolvable
	//	No breed event should have the same name

	// TODO: Second check might also guarantee the first in this implementation

	auto generateCycleErrorMessage = [] (const BreedEventNode *firstNode, const BreedEventNode *secondNode)
	{
		return "Cycle discovered in pedigree (i.e. an organism is its own ancestor). Found back edge between " + firstNode->result().name() + " and " + secondNode->result().name() + ".";
	};

	auto generateDuplicateErrorMessage = [](const BreedEventNode &node)
	{
		return "Organism designated as " + node.result().name() + " was produced more than once in the pedigree.";
	};

	std::map<std::string, const BreedEventNode*> namesAndBreedEventsFound;

	std::function<Maybe<std::string> (const BreedEventNode*)> checkDependencies = [&] (const BreedEventNode *node)
	{
		Maybe<std::string> returnValue;

		// Check that we haven't seen this name produced by a different breed event elsewhere
		try
		{
			if (*namesAndBreedEventsFound.at(node->result().name()) != *node)
			{
				returnValue.setValue(generateDuplicateErrorMessage(*node));
			}
		}
		catch (std::out_of_range)
		{
			// Not in map so far so add it
			namesAndBreedEventsFound[node->result().name()] = node;
		}

		for (unsigned int i = 0; i < node->numberOfDependencies(); i++)
		{
			// Check for cycle by looking for back edge
			try
			{
				namesAndBreedEventsFound.at(node->dependency(i)->result().name());
			}
			catch (std::out_of_range)
			{
				// If cycle found, produce an error
				returnValue.setValue(generateCycleErrorMessage(node, node->dependency(i)));
				return returnValue;
			}

			returnValue = checkDependencies(node->dependency(i));
		}

		return returnValue;
	};

	Maybe<std::string> returnValue;

	for (auto it = leaves.begin(); it != leaves.end(); it++)
	{
		if (auto error = checkDependencies(&(*it)))
		{
			returnValue.setValue(error.value());
		}
	}

	return returnValue;
}


std::vector<std::string> Pedigree::getNamesOfAllIndividuals()
{
	std::vector<std::string> returnVec;
	std::map<std::string, bool> foundSoFar;

	std::function<void (BreedEventNode*)> addNode = [&] (BreedEventNode* node)
	{
		try
		{
			foundSoFar.at(node->result().name());
		}
		catch (std::out_of_range)
		{
			// If not found then add it
			returnVec.push_back(node->result().name());
			foundSoFar[node->result().name()] = true;
		}

		for (unsigned int i = 0; i < node->numberOfDependencies(); i++)
		{
			addNode(node->dependency(i));
		}
	};

	for (auto it = leaves.begin(); it != leaves.end(); it++)
	{
		addNode(&(*it));
	}

	return returnVec;
}


void Pedigree::evaluate(const Breeder* breeder)
{
	std::function<void (BreedEventNode*, const Breeder*)> recursiveEvaluate = [&](BreedEventNode *node, const Breeder* breeder)
	{
		// Evaluate all dependencies, then evaluate yourself
		for (unsigned int i = 0; i < node->numberOfDependencies(); i++)
		{
			recursiveEvaluate(node->dependency(i), breeder);
		}

		node->breed(breeder);
	};

	for (auto it = leaves.begin(); it != leaves.end(); it++)
	{
		recursiveEvaluate(&(*it), breeder);
	}
}