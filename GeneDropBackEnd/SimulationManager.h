#pragma once
#include "Breeder.h"
#include <map>
#include <memory>
#include "Pedigree.h"
#include "State.h"
#include <thread>


class SimulationManager {
public:
	SimulationManager();
	~SimulationManager();

	static Pedigree buildPedigreeFromFile(const std::string& fileName);

	State getRealisation();

	void build(const std::string& pedigreeFileName, const std::string& lociFileName, const std::string& genotypeFileName);

private:
	std::map<std::thread::id, std::unique_ptr<RNGController>> rngMap;
	std::unique_ptr<Breeder> breeder;
	Pedigree pedigree;
	State startingState;

	static const std::string founderGenerationName;

	void buildStartingStateFromFiles(const std::string& lociFileName, const std::string& founderFileName);
};


