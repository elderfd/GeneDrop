#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RNGController.h"

TEST_CASE("RNGController function")
{
	// How many times to generate random numbers for testing, larger is more rigorous but more slow
	const int repeatNumber = 4;

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
				auto someInt = floor(someDouble);

				REQUIRE(someInt == rng.getUniformlyDistributed(someInt, someInt));
			}

			for (int i = 0; i < repeatNumber; i++)
			{
				int min = floor(rng.getUniformlyDistributed() * 1000);
				int max = min + floor(rng.getUniformlyDistributed() * 1000);

				int randomNumber = rng.getUniformlyDistributed(min, max);

				REQUIRE(randomNumber <= max);
				REQUIRE(randomNumber >= min);
			}
		}
	}
}
