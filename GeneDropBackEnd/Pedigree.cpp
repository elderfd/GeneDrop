#include "Pedigree.h"
#include <functional>
#include "Maybe.h"

Pedigree::Pedigree()
{
}


Pedigree::~Pedigree()
{
}


Maybe<std::string> Pedigree::isUsable() const
{
	// Depth-first recursion and make sure that all dependencies can be resolved
	// Cycles in pedigree are unsolvable 

	auto generateErrorMessage = [] (const BreedEventNode &firstNode, const BreedEventNode &secondNode)
	{
		return "Cycle discovered in pedigree (i.e. an organism is its own ancestor). Found back edge between " + firstNode.name + " and " + secondNode.name + ".";
	};

	std::function<Maybe<std::string> (BreedEventNode)> checkDependencies = [&] (BreedEventNode &node)
	{
		Maybe<std::string> returnValue;

		node.covered = true;

		for (int i = 0; i < node.dependencies.size(); i++)
		{
			// Check for cycle by looking for back edge
			if (node.dependencies[i].covered)
			{
				// If cycle found, produce an error
				returnValue.setValue(generateErrorMessage(node, node.dependencies[i]));
				return returnValue;
			}

			checkDependencies(node.dependencies[i]);
		}

		return returnValue;
	};

	Maybe<std::string> returnValue;

	for (unsigned int i = 0; i < roots.size(); i++)
	{
		if (auto error = checkDependencies(roots[i]))
		{
			returnValue.setValue(error.value());
		}
	}

	return returnValue;
}
