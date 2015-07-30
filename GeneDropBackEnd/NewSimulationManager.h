#pragma once
#include "Breeder.h"
#include <memory>
#include "NewPedigree.h"
#include "State.h"


class NewSimulationManager {
public:
	NewSimulationManager();
	~NewSimulationManager();

	void buildPedigreeFromFile(std::string fileName);
	void buildStartingStateFromFiles(std::string lociFileName, std::string founderFileName);

	State getRealisation();

	// TODO: Find better home for this
	static std::string makeTimeStamp();
private:
	RNGController rng;
	std::unique_ptr<Breeder> breeder;
	NewPedigree pedigree;
	State startingState;
};

