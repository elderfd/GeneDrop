#include "CommandLineParser.h"
#include <initializer_list>

const std::string
	CommandLineParser::pedigreeFileKey = "pedigree",
	CommandLineParser::genotypeFileKey = "genotype",
	CommandLineParser::lociFileKey = "loci",
	CommandLineParser::settingsFileKey = "settings";

const std::vector<std::string> CommandLineParser::allKeys = {
	CommandLineParser::pedigreeFileKey,
	CommandLineParser::genotypeFileKey,
	CommandLineParser::lociFileKey,
	CommandLineParser::settingsFileKey
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

	// First see if we've specified a pedigree
	if (auto pedigreeFile = getValueForKey(pedigreeFileKey, argc, argv))
	{
		// Look for the other inputs too then
	}	// If no pedigree, have a look for a settings XML file
	else if (auto settingsFile = getValueForKey(settingsFileKey, argc, argv))
	{
		// If we found a settings file we need to parse it to extract the needed data
	}
	else success = false;

	return success;
}


CommandLineParser::Maybe<std::string> CommandLineParser::getValueForKey(std::string key, int argc, char *argv[])
{
	Maybe<std::string> retVal;

	for (int i = 0; i < argc; i++)
	{
		// TODO: This whole thing could certainly be improved
		if (argv[i][0] == '-' && unambigiousKeyMatch(std::string(argv[i]), key))
		{
			if (++i < argc)
			{
				retVal.setValue(std::string(argv[i]));
			}

			break;
		}
	}

	return retVal;
}


bool CommandLineParser::unambigiousKeyMatch(std::string toCheck, std::string key)
{
	bool matchedRightKey = false;
	bool matchedOtherKey = false;

	for (int i = 0; i < allKeys.size(); i++)
	{
		bool matched = true;

		for (int j = 0; j < toCheck.size(); j++)
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