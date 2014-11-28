#pragma once

#include <string>
#include "Genotype.h"

//! One individual in the system
/*!
	Would be able to use just genomes as individuals but might be interested in phenotypes as well later.
*/
class Organism
{
public:
	Organism();
	~Organism();

	std::string name() const
	{
		return _name;
	}

	void setName(std::string name)
	{
		_name = name;
	}

	unsigned int numberOfChromosomes() const
	{
		return _genotype.numberOfChromosomes();
	}

	const Genotype& genotype() const
	{
		return _genotype;
	}

	Genotype& genotype()
	{
		return _genotype;
	}

	void setGenotype(const Genotype& genotype)
	{
		this->_genotype = genotype;
	}

protected:
	// Identifier for this organism
	std::string _name;

	Genotype _genotype;
};

