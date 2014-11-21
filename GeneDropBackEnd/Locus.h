#pragma once

#include <string>

class Locus
{
public:
	Locus(std::string ID, double geneticDistance, std::string allele);
	Locus();

	std::string getAllele() const;
	double getGeneticDistance() const;
	std::string getID() const;

	void setAllele(std::string allele);
	void setGeneticDistance(double geneticDistance);
	void setID(std::string ID);

	bool operator==(const Locus &other) const;
	bool operator!=(const Locus &other) const;
	bool operator<(const Locus &other) const;

private:
	//! The name of this locus
	std::string ID;

	//! The allele present at this locus
	std::string allele;

	//! Distance from end of the chromosome
	double geneticDistance;
};

