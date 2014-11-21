#include "SimulationManagerFactory.h"
#include <fstream>


SimulationManagerFactory::SimulationManagerFactory()
{
}


SimulationManagerFactory::~SimulationManagerFactory()
{
}


SimulationManager SimulationManagerFactory::createFromSimpleInput(std::string pedigreeFileName, std::string genotypeFileName, std::string lociFileName)
{
	// TODO: Implement

	// Start with the pedigree file
	std::ifstream pedigreeFile;
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

		int counter = 0;

		std::string generationName, ID, firstParent, secondParent;

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
				firstParent = token;
			case 3:
				secondParent = token;
			default:
				// Do nothing
				break;
			}

			counter++;
		}

		// Add a new node to the pedigree
	}

	pedigreeFile.close();

	return SimulationManager();
}