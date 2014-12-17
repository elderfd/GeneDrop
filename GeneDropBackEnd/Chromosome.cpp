#include "Chromosome.h"
#include <stdlib.h>
#include <stdexcept>
#include <cassert>

Chromosome::Chromosome()
{
}

Chromosome::Chromosome(int numberOfLociIn)
{
	loci.resize(numberOfLociIn);
}

Chromosome::~Chromosome()
{
}

void Chromosome::setAllele(int locus, std::string value)
{
	loci[locus].setAllele(value);
}

std::string Chromosome::getAllele(int locus) const
{
	return loci[locus].getAllele();
}

void Chromosome::setGeneticDistance(int locusIndex, double value)
{
	loci[locusIndex].setGeneticDistance(value);
}

double Chromosome::getGeneticDistance(int locusIndex) const
{
	return loci[locusIndex].getGeneticDistance();
}

int Chromosome::getNumberOfLoci() const
{
	return loci.size();
}

bool Chromosome::operator==(const Chromosome &other) const
{
	if (loci.size() != other.loci.size())
	{
		return false;
	}

	for (unsigned int i = 0; i < loci.size(); i++)
	{
		if (loci[i].getAllele() != other.loci[i].getAllele())
		{
			return false;
		}
	}

	return true;
}

bool Chromosome::operator<(const Chromosome& otherChromosome) const
{
	assert(otherChromosome.getNumberOfLoci() == getNumberOfLoci());

	for (unsigned int i = 0; i < loci.size(); i++)
	{
		if (loci[i].getAllele()<otherChromosome.getAllele(i))
		{
			return true;
		}
		if (loci[i].getAllele() != otherChromosome.getAllele(i))
		{
			return false;
		}
	}

	return false;
}

bool Chromosome::operator!=(const Chromosome& otherChromosome) const
{
	if ((*this) == otherChromosome)
	{
		return false;
	}

	return true;
}

void Chromosome::setGeneticDistance(std::string locusID, double value)
{
	int locusIndex = getLocusIndexFromID(locusID);

	setGeneticDistance(locusIndex, value);
}

int Chromosome::getLocusIndexFromID(std::string locusID) const
{
	//Linear search for the moment
	for (unsigned int i = 0; i<loci.size(); i++)
	{
		if (loci[i].getID() == locusID)
		{
			return i;
		}
	}

	throw std::runtime_error("Error: Could not find locus with ID " + locusID);
}

void Chromosome::addLocus(const Locus &newLocus)
{
	loci.push_back(newLocus);
}

double Chromosome::getGeneticDistance(std::string locusID) const
{
	int index = getLocusIndexFromID(locusID);

	return loci[index].getGeneticDistance();
}


std::string Chromosome::getID() const
{
	return ID;
}

void Chromosome::setID(std::string newID)
{
	ID = newID;
}

Locus Chromosome::getLocusCopy(int locusIndex) const
{
	return loci[locusIndex];
}

void Chromosome::setAllele(std::string locusID, std::string value)
{
	int locusIndex = getLocusIndexFromID(locusID);

	loci[locusIndex].setAllele(value);
}


double Chromosome::distanceBetweenLoci(int firstLocus, int secondLocus) const
{
	return abs(loci[firstLocus].getGeneticDistance() - loci[secondLocus].getGeneticDistance());
}


double Chromosome::distanceBetweenLoci(std::string firstLocus, std::string secondLocus) const
{
	return distanceBetweenLoci(getLocusIndexFromID(firstLocus), getLocusIndexFromID(secondLocus));
}


const Locus& Chromosome::locus(int index) const
{
	return loci[index];
}


std::string Chromosome::getLocusID(int locusIndex) const
{
	return loci[locusIndex].getID();
}