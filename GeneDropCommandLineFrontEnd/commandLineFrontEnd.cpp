#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "SimulationManagerFactory.h"
#include "SimulationManager.h"
#include "CommandLineParser.h"

int main(int argc, char *argv[])
{
	int returnVal = 0;

	CommandLineParser cLineParser;

	// Add all of the parameters we need
	std::string pedigreeFileName, genotypeFileName, lociFileName;
	int numberOfRuns, numberOfThreads;
	bool printHelp = false;

	cLineParser.addArg("pedigree", &pedigreeFileName);
	cLineParser.addArg("genotype", &genotypeFileName);
	cLineParser.addArg("loci", &lociFileName);
	cLineParser.addArg("numberOfRuns", &numberOfRuns);
	cLineParser.addArg("numberOfThreads", &numberOfThreads, 1);
	cLineParser.addArg("help", &printHelp, false);

	// Now parse the input
	cLineParser.parse(argc, argv);

	// First check if we asked for help printing
	if (printHelp)
	{
		std::string errorMessage = "Input syntax is options and values. Available options are, \n";

		auto addCommandLineOptSpec = [&](std::string option, std::string meaning)
		{
			errorMessage += "\t-" + option + "  =  " + meaning + "\n";
		};

		addCommandLineOptSpec("pedigree", "The path to the pedigree file to use.");
		addCommandLineOptSpec("genotype", "The path to the founder genotypes file to use.");
		addCommandLineOptSpec("loci", "The path to the loci file to use.");
		addCommandLineOptSpec("numberOfRuns", "The number of simulations to carry out for this dataset.");
		addCommandLineOptSpec("numberOfThreads", "The number of threads to use for simulations.");
		addCommandLineOptSpec("help", "Print help message for the program.");

		std::cout << errorMessage << std::endl;

		return 0;
	}

	// TODO: Put input verification in

	// Check for any errors
	bool shouldExit = false;

	if (auto errors = cLineParser.errorsEncountered())
	{
		std::cout << "Error in parsing input: " << std::endl;

		for (auto message : errors.value())
		{
			std::cout << message << std::endl;
		}

		shouldExit = true;
	}

	if (auto warnings = cLineParser.warningsEncountered())
	{
		std::cout << "Warnings generated when parsing input: " << std::endl;

		for (auto message : warnings.value())
		{
			std::cout << message << std::endl;
		}
	}

	if (shouldExit)
	{
		return 1;
	}

	// Then set up an appropriate simulation state
	SimulationManager simManager = SimulationManagerFactory::createFromSimpleInput(
		pedigreeFileName,
		genotypeFileName,
		lociFileName,
		numberOfRuns,
		numberOfThreads
	);

	// Verify the input
	Maybe<std::string> error = simManager.verifySimulationPrototype();

	if (error)
	{
		// Push out any error message
		std::cerr << error.value() << std::endl;
		returnVal = 1;
	}
	else
	{
		// Do some sims
		simManager.run();

		// Get some output
		std::string outputFileName = "Output(" + SimulationManager::makeTimeStamp() + ").csv";

		simManager.outputResultsToFile(outputFileName);
	}

	// Exit cleanly
	return returnVal;
}