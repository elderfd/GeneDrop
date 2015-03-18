#pragma once

#include <vector>
#include "Chromosome.h"

//! Stores an organism genotype
class Genotype {
public:
	Genotype();
	~Genotype();

	//! Homologous set of chromosomes
	class Homology {
	public:

		unsigned int ploidy() const {
			return chromosomes.size();
		}

		std::vector<Chromosome> chromosomes;
	};

	unsigned int ploidy() const {
		// Should all have same ploidy - can ensure is true on construction
		return homologousChromosomes[0].ploidy();
	}

	unsigned int numberOfChromosomes() const {
		return homologousChromosomes.size();
	}

	void clear() {
		homologousChromosomes.clear();
	}

	//! Returns the allele value at a given chromosome and locus.
	/*!
		set controls which of the homologous chromosomes is inspected.
		*/
	std::string allele(int chromosomeIndex, int locusIndex, int set) const;

	//! Sets the allele value at a given chromosome and locus.
	/*!
		set controls which of the homologous chromosomes is affected.
		*/
	void setAllele(int chromosomeIndex, int locusIndex, int set, std::string allele);

	std::pair<int, int> getChromosomeAndLocusIndexForLocus(std::string locusID) const;

	void addHomologousChromosomes(const std::vector<Chromosome> &chromosomes);
	void addHomologousChromosomes(const Homology& homology);

	const Chromosome& chromosome(unsigned int setIndex, unsigned int chromosomeIndex) const;

	unsigned int numberOfLoci(int chromosomeIndex) const;

protected:
	// Sets of homologous chromosomes
	std::vector<Homology> homologousChromosomes;
};
