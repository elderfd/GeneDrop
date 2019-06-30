#include "Genotype.h"
#include <cassert>
#include <sstream>


Genotype::Genotype() {}


Genotype::~Genotype() {}


const Chromosome& Genotype::chromosome(const size_t setIndex, const size_t chromosomeIndex) const {
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


std::string Genotype::allele(const size_t chromosomeIndex, const size_t locusIndex, const size_t set) const {
	return homologousChromosomes[chromosomeIndex].chromosomes[set].getAllele(locusIndex);
}


void Genotype::setAllele(const size_t chromosomeIndex, const size_t locusIndex, const size_t set, std::string allele) {
	homologousChromosomes[chromosomeIndex].chromosomes[set].setAllele(locusIndex, allele);
}


std::pair<const size_t, const size_t> Genotype::getChromosomeAndLocusIndexForLocus(std::string locusID) const {
	for (size_t i = 0; i < homologousChromosomes.size(); i++) {
		for (std::size_t j = 0; j < homologousChromosomes[i].chromosomes[0].getNumberOfLoci(); j++) {
			if (homologousChromosomes[i].chromosomes[0].getLocusID(j) == locusID) {
				return std::pair<const size_t, const size_t>(i, j);
			}
		}
	}

	std::stringstream errorMessage;
	errorMessage << "Unknown locus "
		<< locusID
		<< " requested.";

	throw std::runtime_error(errorMessage.str());
}


size_t Genotype::numberOfLoci(int chromosomeIndex) const {
	return homologousChromosomes[chromosomeIndex].chromosomes[0].getNumberOfLoci();
}