#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RNGController.h"

TEST_CASE("RNGController function")
{
	// Set up test
	unsigned long long seed = 67;
	RNGController rng(seed);

	REQUIRE(rng.seed() == seed);

	SECTION("We are able to reseed the generator.")
	{
		std::vector<double> firstSequence;

		for (int i = 0; i < 1000; i++)
		{
			firstSequence.push_back(rng.getUniformlyDistributed());
		}

		rng.reseed(seed);

		REQUIRE(rng.seed() == seed);

		std::vector<double> secondSequence;

		for (int i = 0; i < 1000; i++)
		{
			secondSequence.push_back(rng.getUniformlyDistributed());
		}

		REQUIRE(firstSequence == secondSequence);
	}
}
