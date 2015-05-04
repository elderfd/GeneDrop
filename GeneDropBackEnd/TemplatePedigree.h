#pragma once
#include <vector>
#include "IndividualNode.h"
#include "OrganismSpecifier.h"
#include "OrganismManager.h"
#include "PedigreeRealisation.h"


//! The structure of a pedigree
class TemplatePedigree {
public:
	TemplatePedigree();
	~TemplatePedigree();

	//! Generates a stochastic realisation of the pedigree
	std::unique_ptr<PedigreeRealisation> generateRealisation() const;

	//! Adds an individual to the pedigree where the individual is the output of a cross
	void addIndividual(std::string name, std::string nameOfFather, std::string nameOfMother);

	// Adds an individual to the pedigree where the individual is the output of selfing
	void addIndividual(std::string name, std::string nameOfParent);

private:
	// The leaves of the pedigree (i.e. the individuals without children specified as yet)
	std::vector<std::unique_ptr<IndividualNode>> leaves;

	// Below are factory methods for the nodes
	//! Builds an individual who has no parents specified
	static std::unique_ptr<IndividualNode> buildOrphan();

	//! Builds an individual with the given parents - mother/father essentially interchangeable for now
	static std::unique_ptr<IndividualNode> buildIndividualWithKnownParents(const OrganismSpecifier& father, const OrganismSpecifier& mother);

	//! Builds an individual where the parents belong to certain groups but it's not clear which individual
	/*!
	Can also supply a name for either parent to only randomise one
	*/
	static std::unique_ptr<IndividualNode> buildIndividualFromUncertainCross(const OrganismSpecifier& fatherCandidateGroup, const OrganismSpecifier& motherCandidateGroup);

	//! Builds an individual where the single parent belongs to a particular group
	static std::unique_ptr<IndividualNode> buildIndividualFromUncertainSelf(const OrganismSpecifier&parentCandidateGroup);

	//! Makes this node into a founder node with the supplied genotype
	void makeFounder(OrganismSpecifier &node, const Genotype& genotype);

	//! Manages all internal references to individuals
	std::shared_ptr<OrganismManager> organismSpecifierManager;
};

