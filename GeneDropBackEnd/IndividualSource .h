#pragma once
#include <memory>
#include "Organism.h"


//! Any place/event can provide an individual
class IndividualSource {
public:
	IndividualSource();
	~IndividualSource();

	virtual std::unique_ptr<Organism> yieldIndividual() = 0;
};


//! A cross that produces an individual
class Cross : public IndividualSource {
public:
	static std::unique_ptr<Cross> biparentalCross(mother, father);
	static std::unique_ptr<Cross> self(mother);

private:
	Cross();
};
