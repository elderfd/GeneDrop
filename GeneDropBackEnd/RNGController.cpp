#include "RNGController.h"

RNGController::RNGController() : engine((const uint32_t)produceSeed()), uniformGenerator(engine, uniformDistribution)
{
}


RNGController::RNGController(unsigned long long seed) : engine((const uint32_t)seed), uniformGenerator(engine, uniformDistribution)
{
	_seed = seed;
}


RNGController::~RNGController()
{
}


unsigned long long RNGController::produceSeed()
{
	unsigned long long seed;

#ifdef _WIN32
	seed = __rdtsc();
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

	// Scale to [0, 2 + max - min]
	rawRandom *= 2 + max - min;

	// Scale to [min, max + 1)
	rawRandom += min;

	// Convert to int in [min, max]
	return (int)floor(rawRandom);
}


unsigned long long RNGController::seed() const
{
	return _seed;
}


void RNGController::reseed(unsigned long long newSeed)
{
	// Change the internal seed
	_seed = newSeed;
	engine.seed(_seed);

	// Now update all of the generators
	uniformGenerator.distribution().reset();
}