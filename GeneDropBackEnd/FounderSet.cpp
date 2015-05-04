#include "FounderSet.h"


FounderSet::FounderSet() {}


FounderSet::~FounderSet() {}


void FounderSet::addFounder(std::shared_ptr<Organism> founder, std::string unitIdentifier) {
	for (auto& unit : units) {
		if (unit.ID() == unitIdentifier) {
			
		}
	}
}