#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RNGController.h"
#include "Breeder.h"

TEST_CASE("RNGController function")
{
	// How many times to generate random numbers for testing, larger is more rigorous but more slow
	const int repeatNumber = 1000;

	SECTION("")
	{
		// Set up test
		RNGController rng;
		RNGController::SEED_TYPE seed = rng.seed();

		REQUIRE(rng.seed() == seed);

		std::vector<double> testSequence;

		for (int i = 0; i < repeatNumber; i++)
		{
			testSequence.push_back(rng.getUniformlyDistributed());
		}

		SECTION("We are able to reseed the generator.")
		{
			rng.reseed(seed);

			REQUIRE(rng.seed() == seed);

			std::vector<double> secondSequence;

			for (int i = 0; i < repeatNumber; i++)
			{
				secondSequence.push_back(rng.getUniformlyDistributed());
			}

			CAPTURE(seed);
			REQUIRE(testSequence == secondSequence);
		}

		SECTION("We can copy generators and get same output")
		{
			std::vector<double> newSequence;

			SECTION("Copy constructor works.")
			{
				RNGController rngCopy(rng);

				for (int i = 0; i < repeatNumber; i++)
				{
					newSequence.push_back(rngCopy.getUniformlyDistributed());
				}

				REQUIRE(testSequence == newSequence);
			}

			SECTION("Assignment works.")
			{
				RNGController rngCopy = rng;

				for (int i = 0; i < repeatNumber; i++)
				{
					newSequence.push_back(rngCopy.getUniformlyDistributed());
				}

				REQUIRE(testSequence == newSequence);
			}
		}

		SECTION("Integer generation is within right range.")
		{
			for (auto someDouble : testSequence)
			{
				int someInt = (int)floor(someDouble);

				REQUIRE(someInt == rng.getUniformlyDistributed(someInt, someInt));
			}

			for (int i = 0; i < repeatNumber; i++)
			{
				int min = (int)floor(rng.getUniformlyDistributed() * 1000);
				int max = min + (int)floor(rng.getUniformlyDistributed() * 1000);

				int randomNumber = rng.getUniformlyDistributed(min, max);

				REQUIRE(randomNumber <= max);
				REQUIRE(randomNumber >= min);
			}
		}
	}
}

TEST_CASE("Testing the Breeder and sub-classes")
{
	RNGController rng;
	const int repeatNumber = 50000;

	SECTION("Testing the switcher")
	{
		Chromosome testChromosome1, testChromosome2;
		Breeder::ParentalChromosomeSwitcher switcher(&testChromosome1, &testChromosome2, rng);

		const double comparisonTolerance = 0.01;
		const Chromosome* lastChromosomePtr = switcher.chromosome();

		for (double p = 0; p <= 0.5; p += 0.01)
		{
			int switchCounter = 0;

			for (int i = 0; i < repeatNumber; i++)
			{
				switcher.switchWithProbability(p);

				if (lastChromosomePtr != switcher.chromosome()) switchCounter++;

				lastChromosomePtr = switcher.chromosome();
			}

			double switchFrequency = (double)switchCounter / (double)repeatNumber;

			REQUIRE(switchFrequency <= p + comparisonTolerance);
			REQUIRE(switchFrequency >= p - comparisonTolerance);
		}
	}

	SECTION("Testing the Haldane Breeder.")
	{
		HaldaneBreeder breeder(&rng);
		const double maxDistance = 200;
		const double distanceStep = 0.5;

		SECTION("Recombination probabilities should scale properly with distance")
		{
			SECTION("Recombination probability should increase with distance")
			{
				double greatestPSoFar = -1;

				for (double distance = 0; distance < maxDistance; distance += distanceStep)
				{
					auto p = breeder.distanceToRecombinationProbability(distance);
					REQUIRE(p >= greatestPSoFar);
					greatestPSoFar = p;
				}
			}

			SECTION("Recombination probabilities should be in [0, 0.5]")
			{
				for (double distance = 0; distance < maxDistance; distance += distanceStep)
				{
					auto p = breeder.distanceToRecombinationProbability(distance);
					REQUIRE(p >= 0);
					REQUIRE(p <= 0.5);
				}
			}
		}
		
		Genotype firstParentGenotype, secondParentGenotype, childGenotype;
		const int numberOfLoci = 100;

		SECTION("Strongly-linked loci should never recombine")
		{
			Chromosome tightlyLinkedOne, tightlyLinkedTwo;
			
			for (unsigned int i = 0; i < numberOfLoci; i++)
			{
				Locus locus("dummy", 0, "A");
				tightlyLinkedOne.addLocus(locus);
				locus.setAllele("B");
				tightlyLinkedTwo.addLocus(locus);
			}

			firstParentGenotype.addHomologousChromosomes({ tightlyLinkedOne, tightlyLinkedOne });
			secondParentGenotype.addHomologousChromosomes({ tightlyLinkedTwo, tightlyLinkedTwo });

			breeder.breed(firstParentGenotype, secondParentGenotype, childGenotype);

			for (unsigned int i = 0; i < childGenotype.numberOfChromosomes(); i++)
			{
				for (unsigned int j = 0; j < childGenotype.ploidy() - 1; j++)
				{
					std::string firstAllele = childGenotype.allele(i, 0, j);

					for (unsigned int k = 0; k < childGenotype.numberOfLoci(i); k++)
					{
						REQUIRE(firstAllele == childGenotype.allele(i, k, j));
					}
				}
			}
		}

		SECTION("Distant loci should recombine frequently")
		{
			Chromosome tightlyLinkedOne, tightlyLinkedTwo;
			const double minimumRecombinationFrequencyForTest = 0.25;

			for (unsigned int i = 0; i < numberOfLoci; i++)
			{
				Locus locus("dummy", i * 1000, "A");
				tightlyLinkedOne.addLocus(locus);
				locus.setAllele("B");
				tightlyLinkedTwo.addLocus(locus);
			}

			firstParentGenotype.addHomologousChromosomes({ tightlyLinkedOne, tightlyLinkedOne });
			secondParentGenotype.addHomologousChromosomes({ tightlyLinkedTwo, tightlyLinkedTwo });

			breeder.breed(firstParentGenotype, secondParentGenotype, childGenotype);

			for (unsigned int i = 0; i < childGenotype.numberOfChromosomes(); i++)
			{
				for (unsigned int j = 0; j < childGenotype.ploidy() - 1; j++)
				{
					std::string lastAllele = childGenotype.allele(i, 0, j);
					int numberOfRecombinations = 0;

					for (unsigned int k = 0; k < childGenotype.numberOfLoci(i); k++)
					{
						if (lastAllele != childGenotype.allele(i, k, j)) numberOfRecombinations++;

						lastAllele = childGenotype.allele(i, k, j);
					}

					double recombinationFrequency = (double)numberOfRecombinations / (double)numberOfLoci;

					REQUIRE(recombinationFrequency >= minimumRecombinationFrequencyForTest);
				}
			}
		}
	}
}
