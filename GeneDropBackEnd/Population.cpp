#include "Population.h"


Population::Population()
{
}


Population::~Population()
{
}


Organism& Population::organismByName(std::string name)
{
	return organisms.at(name);
}