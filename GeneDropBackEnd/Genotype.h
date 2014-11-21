#pragma once

#include <vector>
#include "Chromosome.h"

//! Homologous set of chromosomes
class Homology
{
public:
	unsigned int ploidy() const
	{
		return chromosomes.size();
	}

	std::vector<Chromosome> chromosomes;
};

//! Stores an organism genotype
class Genotype
{
public:
	Genotype();
	~Genotype();

	unsigned int ploidy() const
	{
		// Should all have same ploidy - can ensure is true on construction
		return homologousChromosomes[0].ploidy();
	}

	unsigned int numberOfChromosomes() const
	{
		return homologousChromosomes.size();
	}

	void addHomologousChromosomes(const std::vector<Chromosome> &chromosomes);
	void addHomologousChromosomes(const Homology& homology);

	const Chromosome& chromosome(unsigned int setIndex, unsigned int chromosomeIndex) const;

protected:
	// Sets of homologous chromosomes
	std::vector<Homology> homologousChromosomes;
};
