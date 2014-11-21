#pragma once

#include "Organism.h"
#include <tuple>
#include "RNGController.h"

//! Handles the breeding of two individuals
class Breeder
{
public:
	Breeder(RNGController& rng);
	~Breeder();

	//! Breeds and sets the child genotype 
	void breed(const Organism& firstParent, const Organism& secondParent, Organism& child) const;

	//! Breeds and produces a new child
	Organism breed(const Organism& firstParent, const Organism& secondParent) const;

	virtual Genotype breed(const Genotype& firstParent, const Genotype& secondParent, Genotype& child) const = 0;

protected:
	RNGController &rng;

	class ParentalChromosomeSwitcher
	{
	public:
		ParentalChromosomeSwitcher(const Chromosome* firstParentChromosome, const Chromosome* secondParentChromosome, RNGController &rng) : rng(rng)
		{
			this->firstParentChromosome = firstParentChromosome;
			this->secondParentChromosome = secondParentChromosome;

			// 50% chance of starting with either
			switchWithProbability(0.5);
		}

		void switchWithProbability(double probability)
		{
			if (rng.getUniformlyDistributed() < probability)
			{
				drawingFromFirstParent = !drawingFromFirstParent;
			}
		}

		const Chromosome* chromosome()
		{
			if (drawingFromFirstParent)
			{
				return firstParentChromosome;
			}
			else
			{
				return secondParentChromosome;
			}
		}

	protected:
		const Chromosome* firstParentChromosome;
		const Chromosome* secondParentChromosome;
		RNGController &rng;
		bool drawingFromFirstParent;
	};
};

//! Uses the Haldane mapping function for producing offspring
class HaldaneBreeder : Breeder
{
	Genotype breed(const Genotype& firstParent, const Genotype& secondParent, Genotype& child) const;

	//! Converts distance in centimorgans to probability of recombination
	static double distanceToRecombinationProbability(double distance);
};

// TODO: Implement Kosambi

//! Uses the Kosambi mapping function for producing offspring
//class KosambiBreeder : Breeder
//{
//	Organism breed(const Genotype& firstParent, const Genotype& secondParent) const;
//};

