#include "RNGController.h"

RNGController::RNGController() : engine(_seed = produceSeed()), uniformGenerator(&engine, uniformDistribution)
{
}


RNGController::RNGController(SEED_TYPE seed) : engine(seed), uniformGenerator(&engine, uniformDistribution)
{
	_seed = seed;
}


RNGController RNGController::operator=(RNGController other) const
{
	return other;
}


RNGController::~RNGController()
{
}

RNGController::RNGController(const RNGController& other) : engine(other._seed), uniformGenerator(&engine, uniformDistribution)
{
	_seed = other._seed;
}

RNGController::SEED_TYPE RNGController::produceSeed()
{
	SEED_TYPE seed;

#ifdef _WIN32
	seed = (SEED_TYPE)__rdtsc();
	std::cout << seed << std::endl;
#else
	unsigned int lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	seed = ((unsigned long long)hi << 32) | lo;
#endif

	return seed;
}


double RNGController::getUniformlyDistributed()
{
	return uniformGenerator();
}


int RNGController::getUniformlyDistributed(int min, int max)
{
	// In [0, 1)
	double rawRandom = uniformGenerator();

	// Scale to [0, max - min)
	rawRandom *= 1 + max - min;

	// Scale to [min, max + 1)
	rawRandom += min;

	// Convert to int in [min, max)
	return (int)floor(rawRandom);
}


RNGController::SEED_TYPE RNGController::seed() const
{
	return _seed;
}


void RNGController::reseed(SEED_TYPE newSeed)
{
	// Change the internal seed
	_seed = newSeed;
	engine.seed(_seed);

	// Now update all of the generators
	uniformGenerator.distribution().reset();
}