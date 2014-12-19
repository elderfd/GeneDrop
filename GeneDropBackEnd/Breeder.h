#pragma once

#include "Organism.h"
#include <tuple>
#include "RNGController.h"

//! Handles the breeding of two individuals
class Breeder
{
public:
	Breeder(RNGController* rng);
	~Breeder();

	Breeder(const Breeder& other);

	void setRNG(RNGController* rng);

	virtual Breeder* makeCopy() const = 0;

	//! Breeds and sets the child genotype 
	void breed(const Organism& firstParent, const Organism& secondParent, Organism& child) const;

	//! Breeds and produces a new child
	Organism breed(const Organism& firstParent, const Organism& secondParent) const;

	virtual Genotype breed(const Genotype& firstParent, const Genotype& secondParent, Genotype& child) const = 0;

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

protected:
	RNGController *rng;
};

//! Uses the Haldane mapping function for producing offspring
class HaldaneBreeder : public Breeder
{
public:
	HaldaneBreeder(RNGController* rng) : Breeder(rng)
	{};

	Breeder* makeCopy() const;

	HaldaneBreeder(const HaldaneBreeder& other) : Breeder(other){};

	Genotype breed(const Genotype& firstParent, const Genotype& secondParent, Genotype& child) const;

	//! Converts distance in centimorgans to probability of recombination
	static double distanceToRecombinationProbability(double distance);
};

// TODO: Implement Kosambi

//! Uses the Kosambi mapping function for producing offspring
//class KosambiBreeder : public Breeder
//{
//	Organism breed(const Genotype& firstParent, const Genotype& secondParent) const;
//};

