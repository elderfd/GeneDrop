#pragma once

#include <string>
#include <vector>
#include <assert.h>
#include "Maybe.h"
#include "CommandLineArg.h"

//! Used to extract options from the args supplied to the program
/*!
	Verifies the input.
*/
class CommandLineParser
{
public:
	CommandLineParser(int argc, char *argv[]);
	CommandLineParser(){};
	~CommandLineParser();

	template<class T> void addArg(std::string key, T* parameterToSet);
	template<class T> void addArg(std::string key, T* parameterToSet, T defaultValue);

	//! Parses the supplied input
	void parse(int argc, char *argv[]);

	//! Return any error message encountered while parsing
	Maybe<std::vector<std::string>> errorsEncountered();

	//! Return any warning messages
	Maybe<std::vector<std::string>> warningsEncountered();

protected:	
	Maybe<CommandLineArgInterface*> getUnambiguousKeyMatch(std::string key);

	static const std::string
		pedigreeFileKey,
		genotypeFileKey,
		lociFileKey,
		settingsFileKey,
		numberOfRunsKey,
		numberOfThreadsKey;

	Maybe<std::string> pedigreeFile, lociFile, genotypeFile, numberOfRuns, numberOfThreads;

	static const std::vector<std::string> allKeys;

	// All error messages found while parsing pushed onto here
	std::vector<std::string> errorStack;
	std::vector<std::string> warningStack;

	std::vector<CommandLineArgInterface*> expectedArgs;
};


template<class T> void CommandLineParser::addArg(std::string key, T* parameterToSet)
{
	expectedArgs.push_back(new CommandLineArg<T>(key, parameterToSet));
}


template<class T> void CommandLineParser::addArg(std::string key, T* parameterToSet, T defaultValue)
{
	expectedArgs.push_back(new CommandLineArg<T>(key, parameterToSet, defaultValue));
}