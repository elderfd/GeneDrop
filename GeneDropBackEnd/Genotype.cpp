#include "Genotype.h"


Genotype::Genotype()
{
}


Genotype::~Genotype()
{
}


const Chromosome& Genotype::chromosome(unsigned int setIndex, unsigned int chromosomeIndex) const
{
	return homologousChromosomes[setIndex].chromosomes[chromosomeIndex];
}


void Genotype::addHomologousChromosomes(const std::vector<Chromosome> &chromosomes)
{
	Homology homology;
	homology.chromosomes = chromosomes;
	addHomologousChromosomes(homology);
}


void Genotype::addHomologousChromosomes(const Homology& homology)
{
	homologousChromosomes.push_back(homology);
}