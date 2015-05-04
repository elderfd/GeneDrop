#include "TemplatePedigree.h"


TemplatePedigree::TemplatePedigree() {}


TemplatePedigree::~TemplatePedigree() {}


std::unique_ptr<PedigreeRealisation> TemplatePedigree::generateRealisation() const {
	std::unique_ptr<PedigreeRealisation> realisation(new PedigreeRealisation(this));

	auto concretiseNode = [this, &realisation](const IndividualNode& node, ConcreteIndividualNode* lastNode) {
		auto concrete = node.concretise();

		// TODO: Work out storage

		if (lastNode == nullptr) {
			realisation->leaves.push_back(concrete);
		}
	};

	for (auto& leaf : leaves) {
		ConcreteIndividualNode* lastNode = nullptr;

		concretiseNode(*leaf, lastNode);
	}

	return realisation;
}