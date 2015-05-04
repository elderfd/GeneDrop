#pragma once
#include <vector>
#include <istream>
#include "PedigreeNode.h"
#include "PedigreeOutput.h"


//! Provides a mapping from a set of founders to future generations
class NewPedigree {
public:
	NewPedigree();
	~NewPedigree();

	//! Does all the breeding and returns the output
	std::shared_ptr<PedigreeOutput> getOutput(const FounderSet& founders);

	//! Builds the pedigree from the stream
	friend std::istream& operator>> (std::istream& in, NewPedigree& pedigree);

protected:
	// The leaves of the breeding tree - the breeding products with no children identified
	std::vector<PedigreeNode*> leaves;
};
