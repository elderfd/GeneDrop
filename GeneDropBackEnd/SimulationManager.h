#pragma once
#include "Breeder.h"
#include <memory>
#include "Pedigree.h"
#include "State.h"


class SimulationManager {
public:
	SimulationManager();
	~SimulationManager();

	void buildPedigreeFromFile(std::string fileName);
	void buildStartingStateFromFiles(std::string lociFileName, std::string founderFileName);

	State getRealisation();

	// TODO: Find better home for this
	static std::string makeTimeStamp();
private:
	RNGController rng;
	std::unique_ptr<Breeder> breeder;
	Pedigree pedigree;
	State startingState;
};

