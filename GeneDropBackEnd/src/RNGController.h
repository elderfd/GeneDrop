#pragma once

#include <boost/random.hpp>

//! This handles all of the random number generation needed for a simulation
class RNGController {
public:
	typedef uint32_t SEED_TYPE;
	typedef boost::random::mt19937 RNG_ENGINE;
	typedef boost::random::uniform_01<> UNIFORM_DISTRIBUTION;
	typedef boost::random::normal_distribution<> NORMAL_DISTRIBUTION;
	typedef boost::random::uniform_int_distribution<SEED_TYPE> SEED_DISTRIBUTION;
	typedef boost::random::variate_generator<RNG_ENGINE *, UNIFORM_DISTRIBUTION> UNIFORM_GENERATOR;
	typedef boost::random::variate_generator<RNG_ENGINE *, NORMAL_DISTRIBUTION> NORMAL_GENERATOR;
	typedef boost::random::variate_generator<RNG_ENGINE *, SEED_DISTRIBUTION> SEED_GENERATOR;

	RNGController();
	RNGController(SEED_TYPE seed);
	~RNGController();

	RNGController(const RNGController& other);
	RNGController operator=(RNGController other) const;

	//RNGController& operator=(const RNGController& other);

	//! Returns a double uniformly distributed in [0, 1)
	double getUniformlyDistributed();

	//! Get uniform random integer in range [min, max]
	int getUniformlyDistributed(int min, int max);

	//! Used for resetting the random number generator with a new seed
	void reseed(SEED_TYPE seed);

	//! Used for reseeding with new random seed
	void reseed();

	//! Generates a new seed using the random number generator
	SEED_TYPE produceRandomisedSeed();

	//! Returns the seed that was used
	SEED_TYPE seed() const;

protected:
	RNG_ENGINE engine;

	UNIFORM_DISTRIBUTION uniformDistribution;
	SEED_DISTRIBUTION seedDistribution;

	UNIFORM_GENERATOR uniformGenerator;
	SEED_GENERATOR seedGenerator;

	SEED_TYPE produceSeedFromProcessor();

	SEED_TYPE _seed;
};