#pragma once
#include <memory>
#include "Genotype.h"
#include "OrganismSpecifier.h"


class ConcreteIndividualNode;


//! A stochastic realisation of a pedigree
class PedigreeRealisation {
	friend class TemplatePedigree;

public:
	PedigreeRealisation(const TemplatePedigree* constDataSource);
	~PedigreeRealisation();

private:
	//! Flyweight design pattern for minimising data duplication
	const std::shared_ptr<TemplatePedigree> constDataSource;

	//! The leaves of the concrete pedigree tree
	std::vector<std::unique_ptr<ConcreteIndividualNode>> leaves;
};

