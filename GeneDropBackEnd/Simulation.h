#pragma once
//! A single run of the simulation model
class Simulation
{
public:
	Simulation();
	~Simulation();

	//! Whether or not the simulation has run and finished
	bool finished() const;

protected:
	bool _finished;
};

