#include "SimulationManagerFactory.h"
#include <fstream>
#include <map>
#include <tuple>
#include <initializer_list>


SimulationManagerFactory::SimulationManagerFactory()
{
}


SimulationManagerFactory::~SimulationManagerFactory()
{
}


SimulationManager SimulationManagerFactory::createFromSimpleInput(std::string pedigreeFileName, std::string genotypeFileName, std::string lociFileName)
{
	// TODO: Implement

	SimulationManager newManager;

	// Parse the loci information file
	std::ifstream lociFile;

	Genotype prototypeGenotype;

	try
	{
		lociFile.open(lociFileName);

		std::vector<Chromosome> chromosomes;

		std::string line;

		// Skip header
		std::getline(lociFile, line);

		while (std::getline(lociFile, line))
		{
			int counter = 0;

			std::stringstream lineStream(line);

			double distance;
			std::string chromosome, ID;
			std::string token;

			while (std::getline(lineStream, token, ','))
			{
				switch (counter)
				{
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
			
			for (unsigned int i = 0; i < chromosomes.size(); i++)
			{
				if (chromosomes[i].getID() == chromosome)
				{
					found = true;
					chromosomes[i].addLocus(newLocus);
				}
			}

			// If chromosome didn't exist already then add it
			if (!found)
			{
				chromosomes.emplace_back();
				chromosomes.back().addLocus(newLocus);
			}
		}

		// TODO: Account for varying ploidy
		for (unsigned int i = 0; i < chromosomes.size(); i++)
		{
			// Assume all diploid for the moment
			std::vector<Chromosome> chromosomePair;
			chromosomePair.push_back(chromosomes[i]);
			chromosomePair.push_back(chromosomes[i]);

			prototypeGenotype.addHomologousChromosomes(chromosomePair);
		}

		lociFile.close();
	}
	catch (std::exception &e)
	{
		lociFile.close();
		throw e;
	}

	// Parse the founder line genotypes
	std::ifstream genotypeFile;

	try
	{
		genotypeFile.open(genotypeFileName);

		// Parse the header first
		std::string line;

		bool isHeader = true;

		// Maps the column number to the right chromosome and locus number
		std::map<int, std::pair<int, int>> columnToChromosomeAndLocusMap;

		while (std::getline(genotypeFile, line))
		{
			std::stringstream lineStream(line);
			std::string token;
			int columnNumber = 0;

			if (isHeader)
			{
				// Parse header
				
				// TODO: Should generalise the delimiters used somewhere
				while (std::getline(lineStream, token, ','))
				{
					// Don't need to look at first column as is the founder name
					if (columnNumber != 0)
					{
						columnToChromosomeAndLocusMap[columnNumber] = prototypeGenotype.getChromosomeAndLocusIndexForLocus(token);
					}

					columnNumber++;
				}

				isHeader = false;
			}
			else
			{
				// Parse normal line

				FounderNode newFounder;

				// Give a blank genotype to set alleles on
				newFounder.organism().setGenotype(prototypeGenotype);
				
				while (std::getline(lineStream, token, ','))
				{
					if (columnNumber == 0)
					{
						// Is the founder name
						newFounder.organism().setName(token);
					}
					else
					{
						// Otherwise is an allele
						std::pair<int, int> chromosomeAndLocusIndex = columnToChromosomeAndLocusMap[columnNumber];

						std::string firstAllele, secondAllele;

						// Check for heterozygosity
						if (token.find("/") != std::string::npos)
						{
							// Split on the "/"
							std::string subToken;
							std::stringstream tokenStream(token);

							int counter = 0;

							// TODO: Generalise for different ploidies
							while (std::getline(tokenStream, subToken, '/'))
							{
								if (counter == 0)
								{
									firstAllele = subToken;
								}
								else if (counter == 1)
								{
									secondAllele = subToken;
								}
								else
								{
									break;
								}

								counter++;
							}
						}
						else
						{
							firstAllele = secondAllele = token;
						}

						newFounder.organism().genotype().setAllele(chromosomeAndLocusIndex.first, chromosomeAndLocusIndex.second, 0, firstAllele);
						newFounder.organism().genotype().setAllele(chromosomeAndLocusIndex.first, chromosomeAndLocusIndex.second, 1, secondAllele);
					}

					columnNumber++;
				}

				newManager.prototypePedigree.roots.push_back(newFounder);
			}
		}

		genotypeFile.close();
	}
	catch (std::exception &e)
	{
		genotypeFile.close();
		throw e;
	}

	// Parse the pedigree file
	std::ifstream pedigreeFile;

	try
	{
		pedigreeFile.open(pedigreeFileName);

		std::string line;

		// Skip header
		std::getline(pedigreeFile, line);

		while (std::getline(pedigreeFile, line))
		{
			// Tokenise the line
			std::string token;
			int counter = 0;

			std::stringstream lineStream(line);

			std::string generationName, ID, firstParentName, secondParentName;

			while (std::getline(lineStream, token, ','))
			{
				switch (counter)
				{
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

			// See if this node already exists
			Maybe<PedigreeNode*> child = newManager.prototypePedigree.findNodeByName(ID);
			PedigreeNode* childNode;

			if (child)
			{
				// If it already exists then add the parents to it
				childNode = child.value();
			}
			else
			{
				// Otherwise make a new node that matches it
				newManager.prototypePedigree.population.emplace_back();
			
				childNode = &newManager.prototypePedigree.population.back();
			}

			// See if the parents are in the founder list, otherwise 

			// Get the parents
			Maybe<PedigreeNode*> firstParent = newManager.prototypePedigree.findNodeByName(firstParentName);
			Maybe<PedigreeNode*> secondParent = newManager.prototypePedigree.findNodeByName(secondParentName);

			auto makeNewParentlessNode = [&] (Maybe<PedigreeNode*> nonExistantNode)
			{

			};

			// If parents don't exist then make a node for them
			if (!firstParent)
			{

			}
	

			
		}

		pedigreeFile.close();
	}
	catch (std::exception &e)
	{
		pedigreeFile.close();

		throw e;
	}
	

	

	return SimulationManager();
}