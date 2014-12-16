#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "SimulationManager.h"
#include "CommandLineParser.h"

int main(int argc, char *argv[])
{
	int returnVal = 0;

	CommandLineParser cLineParser;

	try
	{
		cLineParser.parse(argc, argv);
	}
	catch (std::exception& e)
	{
		std::cout << "Error in parsing input: " << e.what() << std::endl;
		returnVal = 1;
		return returnVal;
	}

	// Then set up an appropriate simulation state
	SimulationManager simManager(cLineParser.createSimulationManagerFromInput());

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


		// TODO: Get some output
	}

	// Exit cleanly
	return returnVal;
}