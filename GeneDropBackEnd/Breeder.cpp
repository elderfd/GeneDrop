#include "Breeder.h"
#include <cassert>


Breeder::Breeder(RNGController* rng) {
	this->rng = rng;
}


Breeder::~Breeder() {}


Breeder::Breeder(const Breeder& other) {
	this->rng = other.rng;
}


Breeder* HaldaneBreeder::makeCopy() const {
	return new HaldaneBreeder(*this);
}


void Breeder::setRNG(RNGController* rng) {
	this->rng = rng;
}


void Breeder::breed(const Organism& firstParent, const Organism& secondParent, Organism& child) const {
	breed(firstParent.genotype(), secondParent.genotype(), child.genotype());
}


Organism Breeder::breed(const Organism& firstParent, const Organism& secondParent) const {
	Organism child;
	breed(firstParent, secondParent, child);
	return child;
}


Genotype HaldaneBreeder::breed(const Genotype& firstParent, const Genotype& secondParent, Genotype& child) const {
	// TODO: Since these tests depend on input data - should check this somewhere earlier when data is read in
	assert(firstParent.numberOfChromosomes() == secondParent.numberOfChromosomes());
	assert(firstParent.ploidy() == secondParent.ploidy());

	// Generate a hybrid chromosome from each parent
	auto makeHybrid = [](Chromosome& hybrid, const Genotype& parent, RNGController* rng, int chromosomeIndex) {
		// Randomly choose one of the homologues to start drawing from
		ParentalChromosomeSwitcher chromosomeSwitcher(&parent.chromosome(chromosomeIndex, 0), &parent.chromosome(chromosomeIndex, 1), *rng);

		// Set the new chromosome's name
		hybrid.setID(chromosomeSwitcher.chromosome()->getID());

		// Draw first allele
		hybrid.addLocus(chromosomeSwitcher.chromosome()->getLocusCopy(0));

		// Keep drawing alleles til we can draw no more
		for (int x = 1; x < chromosomeSwitcher.chromosome()->getNumberOfLoci(); x++) {
			// See if we need to recombine
			double distance = chromosomeSwitcher.chromosome()->distanceBetweenLoci(x - 1, x);
			double recombinationProbability = distanceToRecombinationProbability(distance);

			chromosomeSwitcher.switchWithProbability(recombinationProbability);

			// Draw an allele
			hybrid.addLocus(chromosomeSwitcher.chromosome()->getLocusCopy(x));
		}
	};

	for (unsigned int j = 0; j < firstParent.numberOfChromosomes(); j++) {
		// TODO: This bit could be neatened up
		// TODO: Generalise away from diploid breeding

		std::vector<Chromosome> homologousChromosomes;

		Chromosome firstHybrid, secondHybrid;

		makeHybrid(firstHybrid, firstParent, rng, j);
		makeHybrid(secondHybrid, secondParent, rng, j);

		homologousChromosomes.push_back(firstHybrid);
		homologousChromosomes.push_back(secondHybrid);

		child.addHomologousChromosomes(homologousChromosomes);
	}

	return child;
}


double HaldaneBreeder::distanceToRecombinationProbability(double distance) {
	// cM to M
	distance /= 100;

	return 0.5 * (1 - exp(-2 * distance));
}