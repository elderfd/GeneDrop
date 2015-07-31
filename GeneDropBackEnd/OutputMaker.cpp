#include "SimulationManager.h"
#include "OutputMaker.h"
#include "State.h"


OutputMaker::OutputMaker() {}


OutputMaker::~OutputMaker() {
	if (out) {
		close();
	}
}


bool OutputMaker::open(std::string fileName) {
	if (!out.is_open()) {
		out.open(fileName, std::ofstream::out);
	}

	// Exception is only reason for this not to be ok?
	return true;
}


void OutputMaker::close() {
	if (out.is_open()) {
		out.close();
		simulationsSoFar = 0;
	}
}


OutputMaker& operator<<(OutputMaker &out, State &state) {
	if (out.out && state.size() > 0) {
		// Now actually output some data, go through simulations outputting the genotypes

		// Check if we need a header
		if (out.simulationsSoFar == 0) {
			out.writeHeader(state);
		}

		for (auto& generation : state.generations) {
			for (auto& organism : generation.organisms) {
				out.out << out.simulationsSoFar << "," << state.generatingSeed << "," << organism->name();

				const auto& prototypeGenome = state.getPrototypeGenotype();
				unsigned int numberOfChromosomes = prototypeGenome.numberOfChromosomes();

				for (unsigned int chromosome = 0; chromosome < numberOfChromosomes; chromosome++) {
					for (int locus = 0; locus < prototypeGenome.chromosome(chromosome, 0).getNumberOfLoci(); locus++) {
						for (unsigned int copy = 0; copy < prototypeGenome.ploidy(); copy++) {
							out.out << "," << organism->genotype().chromosome(chromosome, copy).locus(locus).getAllele();
						}
					}
				}

				out.out << std::endl;
			}
		}
		
		out.simulationsSoFar++;
	}

	return out;
}


void OutputMaker::writeHeader(const State& state) {
	if (out && out.is_open()) {
		// First timestamp
		std::string timeStamp = SimulationManager::makeTimeStamp();
		out << timeStamp << std::endl;

		// Then some simulation information
		int numberOfLoci = state.numberOfLoci();;
		int numberOfFounders = -1;

		out << "Number of loci," << std::to_string(numberOfLoci) << std::endl;
		out << "Number of founders," << std::to_string(numberOfFounders) << std::endl; // TODO: Reimplement
		out << "Number of generations," << "-" << std::endl;	//TODO: Reimplement this
		out << "Number of runs," << "-" << std::endl; // TODO: Reimplement

		// Some spacer lines
		out << std::endl << std::endl;

		// Now header for the data
		out << "Run Number,Seed,ID";

		// Name all loci
		auto& prototypeGenotype = state.getPrototypeGenotype();

		for (unsigned int chromosome = 0; chromosome < prototypeGenotype.numberOfChromosomes(); chromosome++) {
			for (int locus = 0; locus < prototypeGenotype.chromosome(chromosome, 0).getNumberOfLoci(); locus++) {
				for (unsigned int copy = 0; copy < prototypeGenotype.ploidy(); copy++) {
					out << "," << prototypeGenotype.chromosome(chromosome, 0).locus(locus).getID() << "-" << copy;
				}
			}
		}

		out << std::endl;
	}
}