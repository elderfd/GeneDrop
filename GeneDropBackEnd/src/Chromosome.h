#pragma once

#include <string>
#include <vector>
#include "Locus.h"

class Chromosome {
public:
	Chromosome();
	Chromosome(int numberOfLociIn);
	~Chromosome();

	void setAllele(const size_t  locus, std::string value);
	void setAllele(std::string locusID, std::string value);
	std::string getAllele(const size_t locus) const;

	std::string getID() const;
	void setID(std::string newID);

	void setGeneticDistance(const size_t locusIndex, double value);
	void setGeneticDistance(std::string locusID, double value);
	double getGeneticDistance(const size_t  locusIndex) const;
	double getGeneticDistance(std::string locusID) const;

	double distanceBetweenLoci(const size_t firstLocus, const size_t secondLocus) const;
	double distanceBetweenLoci(std::string firstLocus, std::string secondLocus) const;

	Locus getLocusCopy(const size_t locusIndex) const;
	const Locus& locus(const size_t index) const;

	std::string getLocusID(const size_t locusIndex) const;

	void addLocus(const Locus &newLocus);

	size_t getNumberOfLoci() const;

	bool operator==(const Chromosome &otherChromosome) const;
	bool operator!=(const Chromosome& otherChromosome) const;
	bool operator<(const Chromosome& otherChromosome) const;

	//Converts cM to a recombination rate
	//static double convertGeneticDistanceToRecombinationRate(double geneticDistance);

protected:
	std::vector<Locus> loci;
	std::string ID;

	int getLocusIndexFromID(std::string locusID) const;
};

