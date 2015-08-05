#include "CommandLineParser.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include "OutputMaker.h"
#include "SimulationManager.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	int returnVal = 0;

	CommandLineParser cLineParser;

	// Add all of the parameters we need
	std::string pedigreeFileName,
		genotypeFileName,
		lociFileName,
		outDirectory,
		printList;
	int numberOfRuns, numberOfThreads;
	bool printHelp = false;

	cLineParser.addArg("pedigree", &pedigreeFileName);
	cLineParser.addArg("genotype", &genotypeFileName);
	cLineParser.addArg("loci", &lociFileName);
	cLineParser.addArg("numberOfRuns", &numberOfRuns);
	cLineParser.addArg("numberOfThreads", &numberOfThreads, 1);
	cLineParser.addArg("help", &printHelp, false);
	cLineParser.addArg(std::string("out"), &outDirectory, std::string("."));
	cLineParser.addArg(std::string("print"), &printList, std::string());

	// Now parse the input
	cLineParser.parse(argc, argv);

	cLineParser.setSingleValue("help");

	// First check if we asked for help printing
	if (printHelp) {
		std::string errorMessage = "Input syntax is options and values. Available options are, \n";

		auto addCommandLineOptSpec = [&](std::string option, std::string meaning) {
			errorMessage += "\t-" + option + "  =  " + meaning + "\n";
		};

		addCommandLineOptSpec("pedigree", "The path to the pedigree file to use.");
		addCommandLineOptSpec("genotype", "The path to the founder genotypes file to use.");
		addCommandLineOptSpec("loci", "The path to the loci file to use.");
		addCommandLineOptSpec("numberOfRuns", "The number of simulations to carry out for this dataset.");
		addCommandLineOptSpec("numberOfThreads", "The number of threads to use for simulations.");
		addCommandLineOptSpec("help", "Print help message for the program.");
		addCommandLineOptSpec("out", "The directory to output the results to.");
		addCommandLineOptSpec("print", "A comma-separated list of which generations to produce output for.");

		std::cout << errorMessage << std::endl;

		return 0;
	}

	// Parse which generations to print
	cLineParser.setSingleValue("print");
	std::vector<std::string> generationsToPrint;

	if (!printList.empty()) {
		// Parse out the individual values
		std::string generation;
		bool stripWhiteSpace = true;
		const char sep = ',';

		for (auto character : printList) {
			if (character == sep) {
				if (!generation.empty()) {
					generationsToPrint.push_back(generation);
				}

				generation = "";
				stripWhiteSpace = true;
			} else {
				if (stripWhiteSpace && std::isspace((unsigned char)character)) continue;

				// Otherwise a legit character
				stripWhiteSpace = false;
				generation.push_back(character);
			}
		}

		if (!generation.empty()) generationsToPrint.push_back(generation);
	}

	// TODO: Put input verification in

	// Check for any errors
	bool shouldExit = false;

	if (auto errors = cLineParser.errorsEncountered()) {
		std::cout << "Error in parsing input: " << std::endl;

		for (auto message : errors.value()) {
			std::cout << message << std::endl;
		}

		shouldExit = true;
	}

	if (auto warnings = cLineParser.warningsEncountered()) {
		std::cout << "Warnings generated when parsing input: " << std::endl;

		for (auto message : warnings.value()) {
			std::cout << message << std::endl;
		}
	}

	if (shouldExit) {
		return 1;
	}

	cLineParser.setAllValues();

	// Then set up an appropriate simulation state
	SimulationManager simManager;

	try {
		simManager.buildPedigreeFromFile(pedigreeFileName);
		simManager.buildStartingStateFromFiles(lociFileName, genotypeFileName);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	

	// TODO: Verify the input
	//Maybe<std::string> error = simManager.verifySimulationPrototype();

	//if (error)
	//{
	//	// Push out any error message
	//	std::cerr << error.value() << std::endl;
	//	returnVal = 1;
	//}

	// TODO: This should be done elsewhere
	std::string outputFileName = outDirectory + "/Output(" + SimulationManager::makeTimeStamp() + ").csv";
	OutputMaker out;
	int reportEvery = 50;

	if (generationsToPrint.size() > 0) out.printOnlyCertainGenerations(generationsToPrint);

	if (out.open(outputFileName)) {
		try {
			for (int i = 0; i < numberOfRuns; i++) {
				auto result = simManager.getRealisation();

				out << result;

				if (i % reportEvery == 0 || i == numberOfRuns - 1) {
					std::cout << "Done " << i + 1 << " runs out of " << numberOfRuns;
				}
			}
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		
		out.close();
	}

	// Exit cleanly
	return returnVal;
}