#include <algorithm>
#include <functional>
#include <numeric>
#include <set>
#include "Pedigree.h"


Pedigree::Pedigree() {}


Pedigree::~Pedigree() {}


void Pedigree::addCross(const OrganismSpecifier& father, const OrganismSpecifier& mother, const OrganismSpecifier& child) {
	// Add the child to the list of nodes
	addNode(nodes, child);

	// Insert the cross in the adjacency matrix
	auto& existing = getSet(edges, child);
	existing.insert(father);
	existing.insert(mother);

	// Add the mother and father if they don't exist already
	addNode(nodes, mother);
	addNode(nodes, father);

	mustUpdateCrossOrder = true;
	pedigreeSize++;
}


void Pedigree::prepareForUse() {
	if (mustUpdateCrossOrder) {
		updateCrossOrder();
	}
}


void Pedigree::updateCrossOrder() {
  	crossOrder.clear();

	// Try to find a path through the pedigree
	// Use Kahn 1962 algorithm which works if pedigree is DAG (and it has to be if built correctly)

	auto allEdges = edges;

	// TODO: Could maintain this set on construction of the pedigree
	auto nodesWithNoIncomingEdges = nodes;

	for (const auto& node : nodes) {
		const auto& connectsTo = edges[node];

		for (const auto& endNode : connectsTo) {
			// In-built count doesn't consider weak equality
			for (const auto& unconnectedNode : nodesWithNoIncomingEdges) {
				if (unconnectedNode == endNode) {
					nodesWithNoIncomingEdges.erase(unconnectedNode);
					break;
				}
			}
		}
	}

	std::vector<OrganismSpecifier> organismProductionOrder;

	while (nodesWithNoIncomingEdges.size() > 0) {
		// Pop an unconnected node
		auto node = *nodesWithNoIncomingEdges.begin();
		nodesWithNoIncomingEdges.erase(node);

		organismProductionOrder.push_back(node);

		auto &dependsOn = getSet(allEdges, node);

		while (dependsOn.size() > 0) {
			// Pop one of the dependencies
			auto m = *(dependsOn.begin());
			dependsOn.erase(m);

			bool hasIncoming = false;

			for (const auto& edgePair : allEdges) {
				for (const auto& second : edgePair.second) {
					if (second == m) {
						hasIncoming = true;
						break;
					}
				}
			}

			if (!hasIncoming) {
				addNode(nodesWithNoIncomingEdges, m);
			}
		}

	}

	// If any edges left then we have a cycle
	auto numberOfEdgesLeft = std::accumulate(
		allEdges.begin(),
		allEdges.end(),
		std::size_t{ 0 },
		[](size_t total, const std::pair<OrganismSpecifier, std::unordered_set<OrganismSpecifier>>& connections) -> size_t {
			return total + connections.second.size();
	});

	if (numberOfEdgesLeft > 0) {
		throw std::runtime_error("Found cycle in pedigree.");
	}

	// Now convert to the crosses that need to be done
	for (const auto& organism : organismProductionOrder) {
		// TODO: maybe worry later about genders

		const auto& parents = getSet(edges, organism);

		// TODO: Can make neater
		if (parents.size() > 0) {
			auto mother = *parents.begin();
			auto father = mother;

			if (parents.size() > 1) {
				father = *++parents.begin();
			}

			crossOrder.emplace_back(father, mother, organism);
		}
	}

	mustUpdateCrossOrder = false;
}


void Pedigree::addNode(std::unordered_set<OrganismSpecifier>& set, const OrganismSpecifier& newNode) {
	// First check for exact match via hash-collision and equality
	if (set.count(newNode) > 0) return;

	// Now check for equality with something existing
	for (auto& node : set) {
		if (node == newNode) {
			// See if the new node adds some more information and take this into account
			if (node.name().empty() && !newNode.name().empty()) {
				set.erase(node);
				set.insert(newNode);
			}

			return;
		}
	}

	// If nothing found then just insert the new node as given
	set.insert(newNode);
}


std::unordered_set<OrganismSpecifier>& Pedigree::getSet(std::unordered_map<OrganismSpecifier, std::unordered_set<OrganismSpecifier>>& map, OrganismSpecifier setSpecifier) {
	// See if the set contains the thing
	try {
		return map.at(setSpecifier);
	} catch (...) {} // Horrible but what else to do?

	// Look with weak equality
	for (auto& pair : map) {
		if (pair.first == setSpecifier) {
			// See if we're adding some new info
			if (pair.first.name().empty() && !setSpecifier.name().empty()) {
				map.erase(pair.first);
				return map[setSpecifier];
			}

			return pair.second;
		}
	}

	// Otherwise insert new element
	return map[setSpecifier];
}


CrossIterator Pedigree::begin() {
	if (mustUpdateCrossOrder) updateCrossOrder();

	return CrossIterator(this);
}


CrossIterator Pedigree::end() const {
	return CrossIterator();
}


CrossIterator::CrossIterator() {}


CrossIterator::CrossIterator(const Pedigree* pedigree) :
	pedigree(pedigree) {
	
	if (pedigree->crossOrder.size() == 0) {
		pedigree = nullptr;
	} else {
		index = pedigree->crossOrder.size() - 1;
	}
}


CrossIterator& CrossIterator::operator++() {
	if (pedigree != nullptr && index == 0) pedigree = nullptr;

	index--;

	return *this;
}


CrossIterator CrossIterator::operator++(int) {
	auto original = *this;
	++(*this);
	return original;
}


const Cross& CrossIterator::operator*() {
	return pedigree->crossOrder[index];
}


bool CrossIterator::operator==(const CrossIterator& other) {
	bool samePedigree = pedigree == other.pedigree;
	bool sameIndex = index == other.index || samePedigree && pedigree == nullptr;

	return samePedigree && sameIndex;
}


bool CrossIterator::operator!=(const CrossIterator& other) {
	return !operator==(other);
}


const Cross* CrossIterator::operator->() {
	return &pedigree->crossOrder[index];
}


std::vector<std::string> Pedigree::getGenerationsContainingName(const std::string& name) const {
	std::vector<std::string> retVec;

	for (const auto& node : nodes) {
		if (node.name() == name && std::find(retVec.begin(), retVec.end(), node.generation()) == retVec.end()) {
			retVec.push_back(node.generation());
		}
	}

	return retVec;
}


std::vector<std::string> Pedigree::getAllGenerations() const {
	std::vector<std::string> retVec;

	for (const auto& node : nodes) {
		if (std::find(retVec.begin(), retVec.end(), node.generation()) == retVec.end()) {
			retVec.push_back(node.generation());
		}
	}

	return retVec;
}