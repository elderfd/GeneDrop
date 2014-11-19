#pragma once

#include <string>
#include <vector>
#include <assert.h>
#include "SimulationManager.h"
#include "Maybe.h"

//! Used to extract options from the args supplied to the program
/*!
	Verifies the input.
*/
class CommandLineParser
{
public:
	CommandLineParser(int argc, char *argv[]);
	~CommandLineParser();

	SimulationManager createSimulationManagerFromInput();

protected:
	// Parses the input and returns true if successful
	bool parse(int argc, char *argv[]);

	// Finds the value matching a key from the input
	Maybe<std::string> getValueForKey(std::string key, int argc, char *argv[]);

	// Returns if toCheck unambiguously matches key or not
	bool unambigiousKeyMatch(std::string toCheck, std::string key);

	static const std::string
		pedigreeFileKey,
		genotypeFileKey,
		lociFileKey,
		settingsFileKey;

	static const std::vector<std::string> allKeys;
};

