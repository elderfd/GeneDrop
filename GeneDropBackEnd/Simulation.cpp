#include "Simulation.h"


Simulation::Simulation()
{
	_finished = false;
}


Simulation::Simulation(const Pedigree& pedigree, const Breeder* breeder)
{
	_finished = false;
	this->pedigree = pedigree.cloneStructureAndInitialState();
	this->breeder = breeder;
}


Simulation::~Simulation()
{
}


bool Simulation::finished() const
{
	return _finished;
}


void Simulation::run()
{

}