#pragma once
#include "Breeder.h"
#include <memory>
#include "Pedigree.h"
#include "State.h"


class SimulationManager {
public:
	SimulationManager();
	~SimulationManager();

	static Pedigree buildPedigreeFromFile(const std::string& fileName);

	State getRealisation();

	void build(const std::string& pedigreeFileName, const std::string& lociFileName, const std::string& genotypeFileName);

	// TODO: Find better home for this
	static std::string makeTimeStamp();
private:
	RNGController rng;
	std::unique_ptr<Breeder> breeder;
	Pedigree pedigree;
	State startingState;

	static const std::string founderGenerationName;

	void buildStartingStateFromFiles(const std::string& lociFileName, const std::string& founderFileName);
};


