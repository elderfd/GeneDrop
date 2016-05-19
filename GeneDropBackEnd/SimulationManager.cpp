#include <chrono>
#include <fstream>
#include "SimulationManager.h"
#include <sstream>


const std::string SimulationManager::founderGenerationName = "Founder";


SimulationManager::SimulationManager() {
	// Default to the Haldane breeder for now
	breeder = std::make_unique<HaldaneBreeder>();
}


SimulationManager::~SimulationManager() {}


Pedigree SimulationManager::buildPedigreeFromFile(const std::string& fileName) {
	// Parse the pedigree file
	std::ifstream pedigreeFile;
	Pedigree pedigree;

	try {
		pedigreeFile.open(fileName);

		if (!pedigreeFile.is_open()) {
			throw std::runtime_error("Could not open pedigree file: " + fileName);
		}

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

			std::string childSpecifier = generationName.empty() ? ID : generationName + "::" + ID;

			pedigree.addCross(
				OrganismSpecifier::fromString(firstParentName),
				OrganismSpecifier::fromString(secondParentName),
				OrganismSpecifier::fromString(childSpecifier)
			);
		}

		pedigreeFile.close();
	} catch (std::exception &e) {
		pedigreeFile.close();

		throw e;
	}

	return pedigree;
}


void SimulationManager::buildStartingStateFromFiles(const std::string& lociFileName, const std::string& founderFileName) {
	std::ifstream lociFile;

	Genotype prototypeGenotype;

	try {
		lociFile.open(lociFileName);

		if (!lociFile.is_open()) {
			throw std::runtime_error("Could not open loci file: " + lociFileName);
		}

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

		if (!genotypeFile.is_open()) {
			throw std::runtime_error("Could not open founder file: " + founderFileName);
		}

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

				startingState.addOrganism(newFounder, SimulationManager::founderGenerationName);
			}
		}

		genotypeFile.close();
	} catch (std::exception &e) {
		genotypeFile.close();
		throw e;
	}
}


State SimulationManager::getRealisation() {
	State newState = startingState;

	// Reseed and record the seed
	rngMap[std::this_thread::get_id()] = std::make_unique<RNGController>();
	RNGController& rng = *rngMap[std::this_thread::get_id()];
	rng.reseed();
	newState.seed(rng.seed());

	for (auto crossIt = pedigree.begin(); crossIt != pedigree.end(); crossIt++) {
		auto mother = newState.getMatchingOrganisms(crossIt->mother);
		auto father = newState.getMatchingOrganisms(crossIt->father);

		if (mother.size() != 1 || father.size() != 1) {
			std::string errorMessage;

			if (mother.size() == 0 || father.size() == 0) {
				errorMessage += "Could not find expected organism(s): ";
				errorMessage += mother.size() == 0 ? crossIt->mother.displayString() + ", " : "";
				errorMessage += father.size() == 0 ? crossIt->father.displayString() : "";
			}
			if (mother.size() > 1 || father.size() > 1) {
				if (mother.size() == 0 || father.size() == 0) errorMessage += "\n";

				errorMessage += "Ambiguous reference to organism(s): ";
				errorMessage += mother.size() > 1 ? crossIt->mother.displayString() + ", " : "";
				errorMessage += father.size() > 1 ? crossIt->father.displayString() : "";
			}

			throw std::runtime_error(errorMessage);
		}

		auto child = breeder->breed(*mother[0], *father[0], rng);
		child.setName(crossIt->child.name());

		// Assume that the first generation found is the right one if not specified
		auto generationName = crossIt->child.generation().empty() ? pedigree.getGenerationsContainingName(child.name())[0] : crossIt->child.generation();

		newState.addOrganism(child, generationName);
	}

	return newState;
}

// Disables unsafe warning for localtime
#pragma warning(disable: 4996)

std::string SimulationManager::makeTimeStamp() {
	auto timeNow = std::chrono::system_clock::now();
	std::time_t convertedTime = std::chrono::system_clock::to_time_t(timeNow);

	std::stringstream stampStream;
	stampStream << std::put_time(std::localtime(&convertedTime), "%H-%M-%S@%d-%m-%Y");

	return stampStream.str();
}


void SimulationManager::build(const std::string& pedigreeFileName, const std::string& lociFileName, const std::string& genotypeFileName) {
	pedigree = buildPedigreeFromFile(pedigreeFileName);
	pedigree.prepareForUse();
	buildStartingStateFromFiles(lociFileName, genotypeFileName);
}