#include "CommandLineParser.h"
#include <initializer_list>
#include "Maybe.h"
#include "SimulationManagerFactory.h"
#include <fstream>

const std::string
	CommandLineParser::pedigreeFileKey = "pedigree",
	CommandLineParser::genotypeFileKey = "genotype",
	CommandLineParser::lociFileKey = "loci",
	CommandLineParser::settingsFileKey = "settings",
	CommandLineParser::numberOfRunsKey = "numberOfRuns",
	CommandLineParser::numberOfThreadsKey = "numberOfThreads";

const std::vector<std::string> CommandLineParser::allKeys = {
	CommandLineParser::pedigreeFileKey,
	CommandLineParser::genotypeFileKey,
	CommandLineParser::lociFileKey,
	CommandLineParser::settingsFileKey,
	CommandLineParser::numberOfRunsKey,
	CommandLineParser::numberOfThreadsKey
};

CommandLineParser::CommandLineParser(int argc, char *argv[])
{
	parse(argc, argv);
}


CommandLineParser::~CommandLineParser()
{
}


bool CommandLineParser::parse(int argc, char *argv[])
{
	bool success = true;

	// TODO: Implement some default values

	// First see if we've specified the traditional input files
	
	if ((pedigreeFile = getValueForKey(pedigreeFileKey, argc, argv))
		&& (lociFile = getValueForKey(lociFileKey, argc, argv))
		&& (genotypeFile = getValueForKey(genotypeFileKey, argc, argv))
		&& (numberOfRuns = getValueForKey(numberOfRunsKey, argc, argv))
		&& (numberOfThreads = getValueForKey(numberOfThreadsKey, argc, argv))
		)
	{
		// Check we can open all files
		std::vector<std::string> fileNames = { pedigreeFile.value(), lociFile.value(), genotypeFile.value() };

		for (auto fileName : fileNames)
		{
			std::ifstream testStream(fileName);

			if (!testStream)
			{
				testStream.close();
				throw std::runtime_error("Failed to open file " + fileName);
			}

			testStream.close();
		}

		// Check that the number of runs is an integer > 0
		try
		{
			int test = std::stoi(numberOfRuns.value());

			if (test <= 0)
			{
				throw std::runtime_error("NULL");
			}
		}
		catch (std::exception)
		{
			throw std::runtime_error("Number of runs specified was not an integer > 0.");
		}

		// TODO: Verify the structure of the files
	}	// Otherwise have a look for a settings XML file
	else if (auto settingsFile = getValueForKey(settingsFileKey, argc, argv))
	{
		// If we found a settings file we need to parse it to extract the needed data
		// TODO: Implement this new input file
	}
	else
	{
		success = false;
		std::string errorMessage = "Invalid input syntax. Should provide following options, \n";

		auto addCommandLineOptSpec = [&] (std::string option, std::string meaning)
		{
			errorMessage += "\t-" + option + "  =  " + meaning + "\n";
		};

		addCommandLineOptSpec(pedigreeFileKey, "The path to the pedigree file to use.");
		addCommandLineOptSpec(genotypeFileKey, "The path to the founder genotypes file to use.");
		addCommandLineOptSpec(lociFileKey, "The path to the loci file to use.");
		addCommandLineOptSpec(numberOfRunsKey, "The number of simulations to carry out for this dataset.");
		addCommandLineOptSpec(numberOfThreadsKey, "The number of threads to use for simulations.");

		throw std::runtime_error(errorMessage);
	}

	return success;
}


Maybe<std::string> CommandLineParser::getValueForKey(std::string key, int argc, char *argv[])
{
	Maybe<std::string> retVal;

	for (int i = 0; i < argc; i++)
	{
		// TODO: This whole thing could certainly be improved
		if (argv[i][0] == '-')
		{
			// Strip the dash
			std::string toCheck = std::string(argv[i]);
			toCheck.erase(0, 1);

			if (unambigiousKeyMatch(toCheck, key))
			{
				if (++i < argc)
				{
					retVal.setValue(std::string(argv[i]));
				}

				break;
			}
		}
	}

	return retVal;
}


bool CommandLineParser::unambigiousKeyMatch(std::string toCheck, std::string key)
{
	bool matchedRightKey = false;
	bool matchedOtherKey = false;

	for (unsigned int i = 0; i < allKeys.size(); i++)
	{
		bool matched = true;

		for (unsigned int j = 0; j < toCheck.size(); j++)
		{
			if (toupper(toCheck[j]) != toupper(allKeys[i][j]))
			{
				matched = false;
				break;
			}
		}

		if (allKeys[i] == key &&  matched)
		{
			matchedRightKey = true;
		}
		else if (matched)
		{
			matchedOtherKey = true;
		}
	}

	if (matchedRightKey && !matchedOtherKey)
	{
		return true;
	}
	else
	{
		return false;
	}
}

SimulationManager CommandLineParser::createSimulationManagerFromInput()
{
	SimulationManagerFactory factory;

	auto manager = factory.createFromSimpleInput(
		pedigreeFile.value(),
		genotypeFile.value(),
		lociFile.value(),
		std::stoi(numberOfRuns.value()),
		std::stoi(numberOfThreads.value())
	);

	return manager;
}