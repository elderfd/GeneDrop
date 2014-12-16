#include "Breeder.h"
#include <cassert>


Breeder::Breeder(RNGController* rng)
{
	this->rng = rng;
}


Breeder::~Breeder()
{
}


Breeder::Breeder(const Breeder& other)
{
	this->rng = other.rng;
}


void Breeder::breed(const Organism& firstParent, const Organism& secondParent, Organism& child) const
{
	breed(firstParent.genotype(), secondParent.genotype(), child.genotype());
}


Organism Breeder::breed(const Organism& firstParent, const Organism& secondParent) const
{
	Organism child;
	breed(firstParent, secondParent, child);
	return child;
}


Genotype HaldaneBreeder::breed(const Genotype& firstParent, const Genotype& secondParent, Genotype& child) const
{
	// TODO: Since these tests depend on input data - should check this somewhere earlier when data is read in
	assert(firstParent.numberOfChromosomes() == secondParent.numberOfChromosomes());
	assert(firstParent.ploidy() == secondParent.ploidy());

	for (unsigned int i = 0; i < firstParent.numberOfChromosomes(); i++)
	{
		for (unsigned int j = 0; j < firstParent.numberOfChromosomes(); j++)
		{
			// TODO: This bit could be neatened up

			std::vector<Chromosome> homologousChromosomes;

			// Must produce enough hybrid chromosomes
			for (unsigned int k = 0; k < firstParent.ploidy(); k++)
			{
				Chromosome hybridChromosome;

				// Select a random chromosome from each parent
				int firstParentChromosomeIndex = rng->getUniformlyDistributed(0, firstParent.ploidy());
				int secondParentChromosomeIndex = rng->getUniformlyDistributed(0, secondParent.ploidy());

				// Randomly choose one of the chromosomes to start drawing from
				ParentalChromosomeSwitcher chromosomeSwitcher(&firstParent.chromosome(j, firstParentChromosomeIndex), &secondParent.chromosome(j, secondParentChromosomeIndex), *rng);

				// Draw first allele
				hybridChromosome.addLocus(chromosomeSwitcher.chromosome()->getLocusCopy(0));

				// Keep drawing alleles til we can draw no more
				for (int x = 1; x < chromosomeSwitcher.chromosome()->getNumberOfLoci(); x++)
				{
					// See if we need to recombine
					double distance = chromosomeSwitcher.chromosome()->distanceBetweenLoci(x - 1, x);
					
					// Draw an allele
					hybridChromosome.addLocus(chromosomeSwitcher.chromosome()->getLocusCopy(x));
				}

				homologousChromosomes.push_back(hybridChromosome);
			}
			
			child.addHomologousChromosomes(homologousChromosomes);
		}
	}

	return child;
}


double HaldaneBreeder::distanceToRecombinationProbability(double distance)
{
	// cM to M
	distance /= 100;

	return 0.5 * (1 - exp(-2 * distance));
}