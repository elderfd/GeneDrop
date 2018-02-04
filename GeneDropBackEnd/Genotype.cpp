#include "Genotype.h"
#include <cassert>


Genotype::Genotype() {}


Genotype::~Genotype() {}


const Chromosome& Genotype::chromosome(unsigned int setIndex, unsigned int chromosomeIndex) const {
	return homologousChromosomes[setIndex].chromosomes[chromosomeIndex];
}


void Genotype::addHomologousChromosomes(const std::vector<Chromosome> &chromosomes) {
	Homology homology;
	homology.chromosomes = chromosomes;
	addHomologousChromosomes(homology);
}


void Genotype::addHomologousChromosomes(const Homology& homology) {
	homologousChromosomes.push_back(homology);
}


std::string Genotype::allele(int chromosomeIndex, int locusIndex, int set) const {
	return homologousChromosomes[chromosomeIndex].chromosomes[set].getAllele(locusIndex);
}


void Genotype::setAllele(int chromosomeIndex, int locusIndex, int set, std::string allele) {
	homologousChromosomes[chromosomeIndex].chromosomes[set].setAllele(locusIndex, allele);
}


std::pair<int, int> Genotype::getChromosomeAndLocusIndexForLocus(std::string locusID) const {
	for (unsigned int i = 0; i < homologousChromosomes.size(); i++) {
		for (int j = 0; j < homologousChromosomes[i].chromosomes[0].getNumberOfLoci(); j++) {
			if (homologousChromosomes[i].chromosomes[0].getLocusID(j) == locusID) {
				return std::pair<int, int>(i, j);
			}
		}
	}

	// Shouldn't get here
	assert(false);

	return std::pair<int, int>(-1, -1);
}


size_t Genotype::numberOfLoci(int chromosomeIndex) const {
	return homologousChromosomes[chromosomeIndex].chromosomes[0].getNumberOfLoci();
}