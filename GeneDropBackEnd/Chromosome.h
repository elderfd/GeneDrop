#pragma once

#include <string>
#include <vector>
#include "Locus.h"

class Chromosome {
public:
	Chromosome();
	Chromosome(int numberOfLociIn);
	~Chromosome();

	void setAllele(int locus, std::string value);
	void setAllele(std::string locusID, std::string value);
	std::string getAllele(int locus) const;

	std::string getID() const;
	void setID(std::string newID);

	void setGeneticDistance(int locusIndex, double value);
	void setGeneticDistance(std::string locusID, double value);
	double getGeneticDistance(int locusIndex) const;
	double getGeneticDistance(std::string locusID) const;

	double distanceBetweenLoci(int firstLocus, int secondLocus) const;
	double distanceBetweenLoci(std::string firstLocus, std::string secondLocus) const;

	Locus getLocusCopy(int locusIndex) const;
	const Locus& locus(int index) const;

	std::string getLocusID(int locusIndex) const;

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

