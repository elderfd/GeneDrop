#include "NewPedigree.h"
#include "globals.h"


NewPedigree::NewPedigree() {}


NewPedigree::~NewPedigree() {}


std::istream& operator>> (std::istream& in, NewPedigree& pedigree) {
	std::string line;

	while (std::getline(in, line)) {
		// TODO: Build here!
		std::stringstream lineStream(line);
		std::string token;

		while (std::getline(lineStream, token, Globals::tokeniseOn)) {

		}
	}

	return in;
}