#pragma once
#include <fstream>
#include <string>

class State;

class OutputMaker {
public:
	OutputMaker();
	~OutputMaker();

	//! Opens connection to output file
	bool open(std::string fileName);

	//! Closes connection to output file
	void close();

	friend OutputMaker& operator<<(OutputMaker &out, State &state);

	//! Limits the output to only certain generations
	void printOnlyCertainGenerations(const std::vector<std::string>& toPrint);

	//! Writes an appropriate header to the file
	void writeHeader(const State& state);

private:
	std::ofstream out;
	unsigned int simulationsSoFar = 0;
	std::vector<std::string> generationsToPrint;
};

