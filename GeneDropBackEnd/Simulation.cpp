#include "Simulation.h"


Simulation::Simulation()
{
	_finished = false;

	// Default to Haldane
	this->breeder = new HaldaneBreeder(&rng);
}


Simulation::Simulation(const Pedigree& pedigree, const Breeder* breeder, const RNGController &rng)
{
	_finished = false;
	this->pedigree = pedigree.cloneStructureAndInitialState();
	this->breeder = breeder->makeCopy();
	this->rng = rng;
	this->breeder->setRNG(&this->rng);
}


Simulation::~Simulation()
{
	delete breeder;
}


bool Simulation::finished() const
{
	return _finished;
}


void Simulation::run()
{
	// Evaluate all breeding events in pedigree
	pedigree.evaluate(breeder);

	_finished = true;
}