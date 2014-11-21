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
		return "Cycle discovered in pedigree (i.e. an organism is its own ancestor). Found back edge between " + firstNode->name + " and " + secondNode->name + ".";
	};

	auto generateDuplicateErrorMessage = [](const BreedEventNode &node)
	{
		return "Organism designated as " + node.name + " was produced more than once in the pedigree.";
	};

	std::map<std::string, BreedEventNode> namesAndBreedEventsFound;

	std::function<Maybe<std::string> (const BreedEventNode*)> checkDependencies = [&] (const BreedEventNode *node)
	{
		Maybe<std::string> returnValue;

		// Check that we haven't seen this name produced by a different breed event elsewhere
		try
		{
			if (namesAndBreedEventsFound.at(node->name) != *node)
			{
				returnValue.setValue(generateDuplicateErrorMessage(*node));
			}
		}
		catch (std::out_of_range)
		{
			// Not in map so far so add it
			namesAndBreedEventsFound[node->name] = *node;
		}

		for (unsigned int i = 0; i < node->dependencies.size(); i++)
		{
			// Check for cycle by looking for back edge
			try
			{
				namesAndBreedEventsFound.at(node->dependencies[i]->name);
			}
			catch (std::out_of_range)
			{
				// If cycle found, produce an error
				returnValue.setValue(generateCycleErrorMessage(node, node->dependencies[i]));
				return returnValue;
			}

			returnValue = checkDependencies(node->dependencies[i]);
		}

		return returnValue;
	};

	Maybe<std::string> returnValue;

	for (unsigned int i = 0; i < roots.size(); i++)
	{
		if (auto error = checkDependencies(&roots[i]))
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
			foundSoFar.at(node->name);
		}
		catch (std::out_of_range)
		{
			// If not found then add it
			returnVec.push_back(node->name);
			foundSoFar[node->name] = true;
		}

		for (unsigned int i = 0; i < node->dependencies.size(); i++)
		{
			addNode(node->dependencies[i]);
		}
	};

	for (unsigned int i = 0; i < roots.size(); i++)
	{
		addNode(&roots[i]);
	}

	return returnVec;
}