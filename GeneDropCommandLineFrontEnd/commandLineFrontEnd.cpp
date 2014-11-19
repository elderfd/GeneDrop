#include <stdio.h>
#include <stdlib.h>
#include "SimulationManager.h"
#include "CommandLineParser.h"

int main(int argc, char *argv[])
{
	CommandLineParser cLineParser(argc, argv);

	// Then set up an appropriate simulation state
	SimulationManager simManager = cLineParser.createSimulationManagerFromInput();

	// Do some sims
	simManager.run();

	// Exit cleanly
	return 0;
}