#pragma once
#include "Organism.h"
#include "OrganismSpecifier.h"
#include "PedigreeNode.h"


//! Acts a connector allowing access to parents from multiple different sources
class ParentSupplier {
public:
	ParentSupplier();
	~ParentSupplier();

	OrganismSpecifier get() const;

protected:
};
