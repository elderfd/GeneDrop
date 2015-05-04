#pragma once
#include <memory>
#include <vector>


//! Interface for lazy calculations
class LazyCalculation {
public:
	virtual double calculate() = 0;
};


class LazyAddition : public LazyCalculation {
public:	
	double calculate();

private:
	std::vector<std::shared_ptr<LazyCalculation>> toAdd;
};


class LazyMultiplication : public LazyCalculation {
public:
	double calculate();

private:
	std::vector<std::shared_ptr<LazyCalculation>> toMultiply;
};


class FixedValue : public LazyCalculation {
public:
	FixedValue(double frequency);

	double calculate();

protected:
	double value;
};


//! Specifies a genotype to look for
class GenotypeSpecifier {
public:

};


//! Gives the analytic frequency of a particular genotype
/*!
	Evaluated lazily.
*/
class LazyGenotypeFrequency {
public:
	LazyGenotypeFrequency();
	~LazyGenotypeFrequency();

	bool operator==(const )

private:
	GenotypeSpecifier specifier;

	//! The calculation that is done to get the frequency 
	std::unique_ptr<LazyCalculation> calculation;
};

