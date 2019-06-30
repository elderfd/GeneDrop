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
		size_t ploidy() const {
			return chromosomes.size();
		}

		std::vector<Chromosome> chromosomes;
	};

	size_t ploidy() const {
		// Should all have same ploidy - can ensure is true on construction
		return homologousChromosomes[0].ploidy();
	}

	size_t numberOfChromosomes() const {
		return homologousChromosomes.size();
	}

	void clear() {
		homologousChromosomes.clear();
	}

	//! Returns the allele value at a given chromosome and locus.
	/*!
		set controls which of the homologous chromosomes is inspected.
	*/
	std::string allele(const size_t chromosomeIndex, const size_t locusIndex, const size_t set) const;

	//! Sets the allele value at a given chromosome and locus.
	/*!
		set controls which of the homologous chromosomes is affected.
	*/
	void setAllele(const size_t chromosomeIndex, const size_t locusIndex, const size_t set, std::string allele);

	std::pair<const size_t, const size_t> getChromosomeAndLocusIndexForLocus(std::string locusID) const;

	void addHomologousChromosomes(const std::vector<Chromosome> &chromosomes);
	void addHomologousChromosomes(const Homology& homology);

	const Chromosome& chromosome(const size_t setIndex, const size_t chromosomeIndex) const;

	size_t numberOfLoci(int chromosomeIndex) const;

protected:
	// Sets of homologous chromosomes
	std::vector<Homology> homologousChromosomes;
};
