#define CATCH_CONFIG_MAIN
#include "Breeder.h"
#include "catch.hpp"
#include "SimulationManager.h"
#include "RNGController.h"


TEST_CASE("RNGController function") {
	// How many times to generate random numbers for testing, larger is more rigorous but more slow
	const int repeatNumber = 1000;

	SECTION("") {
		// Set up test
		RNGController rng;
		RNGController::SEED_TYPE seed = rng.seed();

		REQUIRE(rng.seed() == seed);

		std::vector<double> testSequence;

		for (int i = 0; i < repeatNumber; i++) {
			testSequence.push_back(rng.getUniformlyDistributed());
		}

		SECTION("We are able to reseed the generator.") {
			rng.reseed(seed);

			REQUIRE(rng.seed() == seed);

			std::vector<double> secondSequence;

			for (int i = 0; i < repeatNumber; i++) {
				secondSequence.push_back(rng.getUniformlyDistributed());
			}

			CAPTURE(seed);
			REQUIRE(testSequence == secondSequence);
		}

		SECTION("We can copy generators and get same output") {
			std::vector<double> newSequence;

			SECTION("Copy constructor works.") {
				RNGController rngCopy(rng);

				for (int i = 0; i < repeatNumber; i++) {
					newSequence.push_back(rngCopy.getUniformlyDistributed());
				}

				REQUIRE(testSequence == newSequence);
			}

			SECTION("Assignment works.") {
				RNGController rngCopy = rng;

				for (int i = 0; i < repeatNumber; i++) {
					newSequence.push_back(rngCopy.getUniformlyDistributed());
				}

				REQUIRE(testSequence == newSequence);
			}
		}

		SECTION("Integer generation is within right range.") {
			for (auto someDouble : testSequence) {
				int someInt = (int)floor(someDouble);

				REQUIRE(someInt == rng.getUniformlyDistributed(someInt, someInt));
			}

			for (int i = 0; i < repeatNumber; i++) {
				int min = (int)floor(rng.getUniformlyDistributed() * 1000);
				int max = min + (int)floor(rng.getUniformlyDistributed() * 1000);

				int randomNumber = rng.getUniformlyDistributed(min, max);

				REQUIRE(randomNumber <= max);
				REQUIRE(randomNumber >= min);
			}
		}
	}
}


TEST_CASE("Testing the Breeder and sub-classes") {
	RNGController rng;
	const int repeatNumber = 50000;

	SECTION("Testing the switcher") {
		Chromosome testChromosome1, testChromosome2;
		Breeder::ParentalChromosomeSwitcher switcher(&testChromosome1, &testChromosome2, rng);

		const double comparisonTolerance = 0.01;
		const Chromosome* lastChromosomePtr = switcher.chromosome();

		for (double p = 0; p <= 0.5; p += 0.01) {
			int switchCounter = 0;

			for (int i = 0; i < repeatNumber; i++) {
				switcher.switchWithProbability(p);

				if (lastChromosomePtr != switcher.chromosome()) switchCounter++;

				lastChromosomePtr = switcher.chromosome();
			}

			double switchFrequency = (double)switchCounter / (double)repeatNumber;

			REQUIRE(switchFrequency <= p + comparisonTolerance);
			REQUIRE(switchFrequency >= p - comparisonTolerance);
		}
	}

	SECTION("Testing the Haldane Breeder.") {
		HaldaneBreeder breeder;
		const double maxDistance = 200;
		const double distanceStep = 0.5;

		SECTION("Recombination probabilities should scale properly with distance") {
			SECTION("Recombination probability should increase with distance") {
				double greatestPSoFar = -1;

				for (double distance = 0; distance < maxDistance; distance += distanceStep) {
					auto p = breeder.distanceToRecombinationProbability(distance);
					REQUIRE(p >= greatestPSoFar);
					greatestPSoFar = p;
				}
			}

			SECTION("Recombination probabilities should be in [0, 0.5]") {
				for (double distance = 0; distance < maxDistance; distance += distanceStep) {
					auto p = breeder.distanceToRecombinationProbability(distance);
					REQUIRE(p >= 0);
					REQUIRE(p <= 0.5);
				}
			}
		}

		Genotype firstParentGenotype, secondParentGenotype, childGenotype;
		const int numberOfLoci = 100;

		SECTION("Strongly-linked loci should never recombine") {
			Chromosome tightlyLinkedOne, tightlyLinkedTwo;

			for (unsigned int i = 0; i < numberOfLoci; i++) {
				Locus locus("dummy", 0, "A");
				tightlyLinkedOne.addLocus(locus);
				locus.setAllele("B");
				tightlyLinkedTwo.addLocus(locus);
			}

			firstParentGenotype.addHomologousChromosomes({ tightlyLinkedOne, tightlyLinkedTwo });
			secondParentGenotype.addHomologousChromosomes({ tightlyLinkedTwo, tightlyLinkedOne });

			breeder.breed(firstParentGenotype, secondParentGenotype, childGenotype, rng);

			for (unsigned int i = 0; i < childGenotype.numberOfChromosomes(); i++) {
				for (unsigned int j = 0; j < childGenotype.ploidy() - 1; j++) {
					std::string firstAllele = childGenotype.allele(i, 0, j);

					for (unsigned int k = 0; k < childGenotype.numberOfLoci(i); k++) {
						REQUIRE(firstAllele == childGenotype.allele(i, k, j));
					}
				}
			}
		}

		SECTION("Distant loci should recombine frequently") {
			Chromosome unlinkedOne, unlinkedTwo;
			const double minimumRecombinationFrequencyForTest = 0.25;

			for (unsigned int i = 0; i < numberOfLoci; i++) {
				Locus locus("dummy", i * 1000, "A");
				unlinkedOne.addLocus(locus);
				locus.setAllele("B");
				unlinkedTwo.addLocus(locus);
			}

			firstParentGenotype.addHomologousChromosomes({ unlinkedOne, unlinkedTwo });
			secondParentGenotype.addHomologousChromosomes({ unlinkedTwo, unlinkedOne });

			breeder.breed(firstParentGenotype, secondParentGenotype, childGenotype, rng);

			for (unsigned int i = 0; i < childGenotype.numberOfChromosomes(); i++) {
				for (unsigned int j = 0; j < childGenotype.ploidy() - 1; j++) {
					std::string lastAllele = childGenotype.allele(i, 0, j);
					int numberOfRecombinations = 0;

					for (unsigned int k = 0; k < childGenotype.numberOfLoci(i); k++) {
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


TEST_CASE("Testing the output of the simulation with simple cases") {
	std::string testFileFolder = "../TestFiles/";
	int numberOfRuns = 2000;

	SECTION("Biallelic locus segregation in F1 and F2") {
		SimulationManager simManager;
		simManager.build(testFileFolder + "singleBiallelicPedigree.csv", testFileFolder + "singleBiallelicLoci.csv", testFileFolder + "singleBiallelicFounders.csv");

		std::vector<State> allResults;

		for (auto i = 0; i < numberOfRuns; i++) {
			auto result = simManager.getRealisation();

			allResults.push_back(result);
		}

		SECTION("All F1s in this cross must be heterozygous") {
			OrganismSpecifier firstF1("F1-1", ""), secondF1("F1-2", "");

			for (const auto& result : allResults) {
				REQUIRE(result.getMatchingOrganisms(firstF1)[0]->genotype().allele(0, 0, 0) != result.getMatchingOrganisms(firstF1)[0]->genotype().allele(0, 0, 1));
				REQUIRE(result.getMatchingOrganisms(secondF1)[0]->genotype().allele(0, 0, 0) != result.getMatchingOrganisms(secondF1)[0]->genotype().allele(0, 0, 1));
			}
		}

		SECTION("Expecting traditional 1:2:1 ratio in F2") {
			std::map<std::string, double> expectedCounts;
			expectedCounts["00"] = 0.25 * numberOfRuns * 2;
			expectedCounts["01"] = 0.5 * numberOfRuns * 2;
			expectedCounts["11"] = 0.25 * numberOfRuns * 2;

			std::map<std::string, int> genotypeCounts;

			OrganismSpecifier firstF2("F2-1", ""), secondF2("F2-2", "");

			for (const auto& result : allResults) {
				std::string firstGenotype = result.getMatchingOrganisms(firstF2)[0]->genotype().allele(0, 0, 0) + result.getMatchingOrganisms(firstF2)[0]->genotype().allele(0, 0, 1);
				std::string secondGenotype = result.getMatchingOrganisms(secondF2)[0]->genotype().allele(0, 0, 0) + result.getMatchingOrganisms(secondF2)[0]->genotype().allele(0, 0, 1);

				if (firstGenotype == "10") firstGenotype = "01";
				if (secondGenotype == "10") secondGenotype = "01";

				genotypeCounts[firstGenotype]++;
				genotypeCounts[secondGenotype]++;
			}

			// TODO: Move to an actual stats test for this - chi squared would do it
			double percentageErrorTolerance = 10;

			for (const auto& expected : expectedCounts) {
				double absoluteTolerance = percentageErrorTolerance / 100 * expected.second;

				double lowerBound = expected.second - absoluteTolerance;
				double upperBound = expected.second + absoluteTolerance;
				int count = genotypeCounts[expected.first];

				bool testPassed = lowerBound <= count  && upperBound >= count;

				REQUIRE(testPassed);
			}
		}
	}

	SECTION("Biallelic segregation in backcross") {
		SimulationManager simManager;
		simManager.build(testFileFolder + "backcrossPedigree.csv", testFileFolder + "singleBiallelicLoci.csv", testFileFolder + "singleBiallelicFounders.csv");

		std::vector<State> allResults;

		for (auto i = 0; i < numberOfRuns; i++) {
			auto result = simManager.getRealisation();

			allResults.push_back(result);
		}

		SECTION("All F1s in this cross must be heterozygous") {
			OrganismSpecifier F1("F1", "");

			for (const auto& result : allResults) {
				REQUIRE(result.getMatchingOrganisms(F1)[0]->genotype().allele(0, 0, 0) != result.getMatchingOrganisms(F1)[0]->genotype().allele(0, 0, 1));
			}
		}

		SECTION("Expecting 1:1 heterozygote:homozygote ratio in F2") {
			std::map<std::string, double> expectedCounts;
			expectedCounts["00"] = 0.5 * numberOfRuns;
			expectedCounts["01"] = 0.5 * numberOfRuns;

			std::map<std::string, int> genotypeCounts;
			OrganismSpecifier F2("F2", "");

			for (const auto& result : allResults) {
				std::string genotype = result.getMatchingOrganisms(F2)[0]->genotype().allele(0, 0, 0) + result.getMatchingOrganisms(F2)[0]->genotype().allele(0, 0, 1);

				// Careful with heterozygotes
				if (genotype == "10") genotype = "01";

				genotypeCounts[genotype]++;
			}

			// TODO: Move to an actual stats test for this - chi squared would do it
			double percentageErrorTolerance = 10;

			for (const auto& expected : expectedCounts) {
				double absoluteTolerance = percentageErrorTolerance / 100 * expected.second;

				double lowerBound = expected.second - absoluteTolerance;
				double upperBound = expected.second + absoluteTolerance;
				int count = genotypeCounts[expected.first];

				bool testPassed = lowerBound <= count  && upperBound >= count;

				REQUIRE(testPassed);
			}
		}
	}

	SECTION("Simulation can differentiate individuals with the same name in different generations.") {
		SimulationManager simManager;
		simManager.build(testFileFolder + "backcrossDuplicatedNamesPedigree.csv", testFileFolder + "singleBiallelicLoci.csv", testFileFolder + "singleBiallelicFounders.csv");

		std::vector<State> allResults;

		for (auto i = 0; i < numberOfRuns; i++) {
			auto result = simManager.getRealisation();

			allResults.push_back(result);
		}

		SECTION("All F1s in this cross must be heterozygous") {
			OrganismSpecifier F1("", "F1");

			for (const auto& result : allResults) {
				REQUIRE(result.getMatchingOrganisms(F1)[0]->genotype().allele(0, 0, 0) != result.getMatchingOrganisms(F1)[0]->genotype().allele(0, 0, 1));
			}
		}

		SECTION("Expecting 1:1 heterozygote:homozygote ratio in F2") {
			std::map<std::string, double> expectedCounts;
			expectedCounts["00"] = 0.5 * numberOfRuns;
			expectedCounts["01"] = 0.5 * numberOfRuns;

			std::map<std::string, int> genotypeCounts;
			OrganismSpecifier F2("", "F2");

			for (const auto& result : allResults) {
				std::string genotype = result.getMatchingOrganisms(F2)[0]->genotype().allele(0, 0, 0) + result.getMatchingOrganisms(F2)[0]->genotype().allele(0, 0, 1);

				// Careful with heterozygotes
				if (genotype == "10") genotype = "01";

				genotypeCounts[genotype]++;
			}

			// TODO: Move to an actual stats test for this - chi squared would do it
			double percentageErrorTolerance = 10;

			for (const auto& expected : expectedCounts) {
				double absoluteTolerance = percentageErrorTolerance / 100 * expected.second;

				double lowerBound = expected.second - absoluteTolerance;
				double upperBound = expected.second + absoluteTolerance;
				int count = genotypeCounts[expected.first];

				bool testPassed = lowerBound <= count  && upperBound >= count;

				REQUIRE(testPassed);
			}
		}
	}
}
