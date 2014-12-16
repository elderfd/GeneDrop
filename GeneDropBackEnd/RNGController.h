#pragma once

#include <boost/random.hpp>

//! This handles all of the random number generation needed for a simulation
class RNGController
{
	typedef boost::random::mt19937 RNG_ENGINE;
	typedef boost::random::uniform_01<> UNIFORM_DISTRIBUTION;
	typedef boost::random::normal_distribution<> NORMAL_DISTRIBUTION;
	typedef boost::random::variate_generator<RNG_ENGINE *, UNIFORM_DISTRIBUTION> UNIFORM_GENERATOR;
	typedef boost::random::variate_generator<RNG_ENGINE *, NORMAL_DISTRIBUTION> NORMAL_GENERATOR;

public:
	RNGController();
	RNGController(unsigned long long seed);
	~RNGController();

	RNGController(const RNGController& other);
	RNGController& operator=(RNGController other) const;

	//RNGController& operator=(const RNGController& other);

	//! Returns a double uniformly distributed in [0, 1)
	double getUniformlyDistributed();

	//! Get uniform random integer in range [min, max]
	int getUniformlyDistributed(int min, int max);

	//! Used for resetting the random number generator with a new seed
	void reseed(unsigned long long seed);

	// TODO: Implement other distributions

	//! Returns the seed that was used
	unsigned long long seed() const;

protected:
	RNG_ENGINE engine;
	UNIFORM_DISTRIBUTION uniformDistribution;
	UNIFORM_GENERATOR uniformGenerator;

	// TODO: Think about how to handle this in a multithreaded context

	unsigned long long produceSeed();
	unsigned long long _seed;
};