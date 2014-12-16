#include "Pedigree.h"
#include <functional>
#include "Maybe.h"
#include <map>

Pedigree::Pedigree()
{
}


Pedigree::~Pedigree()
{
	for (auto ptr : founders)
	{
		delete ptr;
	}

	for (auto ptr : breedEvents)
	{
		delete ptr;
	}
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

	for (unsigned int i = 0; i < breedEvents.size(); i++)
	{
		recursiveEvaluate(breedEvents[i], breeder);
	}
}


BreedEventNode* Pedigree::addOrganism(std::string name)
{
	auto newNode = new BreedEventNode();
	breedEvents.push_back(newNode);
	breedEvents.back()->organism().setName(name);
	return breedEvents.back();
}


FounderNode* Pedigree::addFounder(std::string name, const Genotype& genotype)
{
	auto newNode = new FounderNode();
	founders.push_back(newNode);
	founders.back()->organism().setName(name);
	founders.back()->organism().setGenotype(genotype);
	return founders.back();
}


Pedigree Pedigree::cloneStructureAndInitialState() const
{
	// Copy first
	Pedigree clone(*this);

	// Then remove the evaluated bits
	for (auto& breedEvent : clone.breedEvents)
	{
		breedEvent->reset();
	}

	return clone;
}


Pedigree::Pedigree(const Pedigree& other)
{
	// Copy across the nodes
	for (auto founder : other.founders)
	{
		this->founders.push_back(new FounderNode(*founder));
	}

	for (auto breedEvent : other.breedEvents)
	{
		this->breedEvents.push_back(new BreedEventNode(*breedEvent));
	}

	// Now need to update all the dependency pointers to actually point to the right thing
	for (unsigned int i = 0; i < other.breedEvents.size(); i++)
	{
		this->breedEvents[i]->dependencies().clear();

		for (auto dependency : other.breedEvents[i]->dependencies())
		{
			// TODO: Could store all of this in a better way I'm sure
			bool found = false;

			for (unsigned int j = 0; j < other.breedEvents.size(); j++)
			{
				if (other.breedEvents[j] == dependency)
				{
					this->breedEvents[i]->dependencies().push_back(new BreedEventNode(*this->breedEvents[j]));
					found = true;
					break;
				}
			}

			for (unsigned int j = 0; j < other.founders.size() && !found; j++)
			{
				if (other.founders[j] == dependency)
				{
					this->breedEvents[i]->dependencies().push_back(new FounderNode(*this->founders[j]));
					found = true;
					break;
				}
			}

			// Check that the pedigree is well-formed, i.e. if dependencies are missing or not
			assert(found);
		}
	}
}