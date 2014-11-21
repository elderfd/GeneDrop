#pragma once

#include <vector>
#include "Chromosome.h"


//! A set of homologous chromosomes
class HomologousSet
{

};


//! Stores an organism genotype
class Genotype
{
public:
	Genotype();
	~Genotype();

protected:
	// First layer is the homologous sets, second layer is the chromosomes
	std::vector<std::vector<Chromosome>> homologousChromosomeSets;
};
