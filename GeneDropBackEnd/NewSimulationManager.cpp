#include <chrono>
#include <fstream>
#include "NewSimulationManager.h"
#include <sstream>


NewSimulationManager::NewSimulationManager() {
	// Default to the Haldane breeder for now
	breeder = std::make_unique<HaldaneBreeder>(&rng);
}


NewSimulationManager::~NewSimulationManager() {}


void NewSimulationManager::buildPedigreeFromFile(std::string fileName) {
	// Parse the pedigree file
	std::ifstream pedigreeFile;

	try {
		pedigreeFile.open(fileName);

		std::string line;

		// Skip header
		std::getline(pedigreeFile, line);

		while (std::getline(pedigreeFile, line)) {
			// Tokenise the line
			std::string token;
			int counter = 0;

			std::stringstream lineStream(line);

			std::string generationName, ID, firstParentName, secondParentName;

			while (std::getline(lineStream, token, ',')) {
				switch (counter) {
				case 0:
					// Generation
					generationName = token;
					// TODO: Do something with the generation name
					break;
				case 1:
					// ID
					ID = token;
					break;
				case 2:
					// First parent
					firstParentName = token;
					break;
				case 3:
					secondParentName = token;
					break;
				default:
					// Do nothing
					break;
				}

				counter++;
			}

			pedigree.addCross(firstParentName, secondParentName, ID);
		}

		pedigreeFile.close();
	} catch (std::exception &e) {
		pedigreeFile.close();

		throw e;
	}
}


void NewSimulationManager::buildStartingStateFromFiles(std::string lociFileName, std::string founderFileName) {
	std::ifstream lociFile;

	Genotype prototypeGenotype;

	try {
		lociFile.open(lociFileName);

		std::vector<Chromosome> chromosomes;

		std::string line;

		// Skip header
		std::getline(lociFile, line);

		while (std::getline(lociFile, line)) {
			int counter = 0;

			std::stringstream lineStream(line);

			double distance;
			std::string chromosome, ID;
			std::string token;

			while (std::getline(lineStream, token, ',')) {
				switch (counter) {
				case 0:
					// Locus ID
					ID = token;
					break;
				case 1:
					// Distance
					distance = std::stod(token);
					break;
				case 2:
					// Chromosome
					chromosome = token;
					break;
				default:
					// Do nothing
					break;
				}

				counter++;
			}

			std::string defaultAllele = "default";

			Locus newLocus(ID, distance, defaultAllele);

			// See if right chromosome already exists
			bool found = false;

			for (unsigned int i = 0; i < chromosomes.size(); i++) {
				if (chromosomes[i].getID() == chromosome) {
					found = true;
					chromosomes[i].addLocus(newLocus);
				}
			}

			// If chromosome didn't exist already then add it
			if (!found) {
				chromosomes.emplace_back();
				chromosomes.back().addLocus(newLocus);
				chromosomes.back().setID(chromosome);
			}
		}

		// TODO: Account for varying ploidy
		for (unsigned int i = 0; i < chromosomes.size(); i++) {
			// Assume all diploid for the moment
			std::vector<Chromosome> chromosomePair;
			chromosomePair.push_back(chromosomes[i]);
			chromosomePair.push_back(chromosomes[i]);

			prototypeGenotype.addHomologousChromosomes(chromosomePair);
		}

		lociFile.close();
	} catch (std::exception &e) {
		lociFile.close();
		throw e;
	}

	// Parse the founder line genotypes
	std::ifstream genotypeFile;

	try {
		genotypeFile.open(founderFileName);

		// Parse the header first
		std::string line;

		bool isHeader = true;

		// Maps the column number to the right chromosome and locus number
		std::map<int, std::pair<int, int>> columnToChromosomeAndLocusMap;

		while (std::getline(genotypeFile, line)) {
			std::stringstream lineStream(line);
			std::string token;
			int columnNumber = 0;

			if (isHeader) {
				// Parse header

				// TODO: Should generalise the delimiters used somewhere
				while (std::getline(lineStream, token, ',')) {
					// Don't need to look at first column as is the founder name
					if (columnNumber != 0) {
						columnToChromosomeAndLocusMap[columnNumber] = prototypeGenotype.getChromosomeAndLocusIndexForLocus(token);
					}

					columnNumber++;
				}

				isHeader = false;
			} else {
				// Parse normal line

				Genotype newFounderGenotype = prototypeGenotype;
				std::string founderName;

				while (std::getline(lineStream, token, ',')) {
					if (columnNumber == 0) {
						// Is the founder name
						founderName = token;
					} else {
						// Otherwise is an allele
						std::pair<int, int> chromosomeAndLocusIndex = columnToChromosomeAndLocusMap[columnNumber];

						std::string firstAllele, secondAllele;

						// Check for heterozygosity
						if (token.find("/") != std::string::npos) {
							// Split on the "/"
							std::string subToken;
							std::stringstream tokenStream(token);

							int counter = 0;

							// TODO: Generalise for different ploidies
							while (std::getline(tokenStream, subToken, '/')) {
								if (counter == 0) {
									firstAllele = subToken;
								} else if (counter == 1) {
									secondAllele = subToken;
								} else {
									break;
								}

								counter++;
							}
						} else {
							firstAllele = secondAllele = token;
						}

						newFounderGenotype.setAllele(chromosomeAndLocusIndex.first, chromosomeAndLocusIndex.second, 0, firstAllele);
						newFounderGenotype.setAllele(chromosomeAndLocusIndex.first, chromosomeAndLocusIndex.second, 1, secondAllele);
					}

					columnNumber++;
				}

				// Build the new individual
				Organism newFounder;
				newFounder.setName(founderName);
				newFounder.setGenotype(newFounderGenotype);

				startingState.addOrganism(newFounder);
			}
		}

		genotypeFile.close();
	} catch (std::exception &e) {
		genotypeFile.close();
		throw e;
	}
}


State NewSimulationManager::getRealisation() {
	State newState = startingState;

	// Reseed and record the seed
	rng.reseed();
	newState.seed(rng.seed());

	for (auto crossIt = pedigree.begin(); crossIt != pedigree.end(); crossIt++) {
		auto cross = *crossIt;

		auto mother = newState.getOrganism(crossIt->motherName);
		auto father = newState.getOrganism(crossIt->fatherName);

		if (!mother || !father) {
			std::string errorMessage = "Could not find expected organism(s): ";

			errorMessage += mother ? "" : crossIt->motherName + " ";
			errorMessage += father ? "" : crossIt->fatherName;

			throw std::runtime_error(errorMessage);
		}

		auto child = breeder->breed(*mother, *father);
		child.setName(crossIt->childName);

		newState.addOrganism(child);
	}

	return newState;
}

// Disables unsafe warning for localtime
#pragma warning(disable: 4996)

std::string NewSimulationManager::makeTimeStamp() {
	auto timeNow = std::chrono::system_clock::now();
	std::time_t convertedTime = std::chrono::system_clock::to_time_t(timeNow);

	std::stringstream stampStream;
	stampStream << std::put_time(std::localtime(&convertedTime), "%H-%M-%S@%d-%m-%Y");

	return stampStream.str();
}