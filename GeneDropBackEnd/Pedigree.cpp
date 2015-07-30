#include <algorithm>
#include "Pedigree.h"
#include <functional>
#include <set>


Pedigree::Pedigree() {}


Pedigree::~Pedigree() {}


void Pedigree::addCross(std::string fatherName, std::string motherName, std::string childName) {
	// Work out if the mother and father already exist within the pedigree
	class IndividualFinder : public NodeChecker {
	public:
		std::string name;
		bool operator()(std::shared_ptr<PedigreeNode> node) {
			return node->cross.childName == name;
		}
	} motherFinder, fatherFinder;

	motherFinder.name = motherName;
	fatherFinder.name = fatherName;
	
	auto motherNode = depthFirstSearchSingleNode(&motherFinder),
		fatherNode = depthFirstSearchSingleNode(&fatherFinder);

	// See if any existing nodes depend on this child
	class ParentChecker : public NodeChecker {
	public:
		std::string name;
		bool operator()(std::shared_ptr<PedigreeNode> node) {
			return node->cross.fatherName == name || node->cross.motherName == name;
		}
	} parentChecker;
	parentChecker.name = childName;

	auto needDependency = depthFirstSearchManyNodes(&parentChecker);

	// Create the new node
	auto newNode = std::make_shared<PedigreeNode>();
	newNode->cross.childName = childName;
	newNode->cross.fatherName = fatherName;
	newNode->cross.motherName = motherName;

	if (motherNode) {
		newNode->dependencies.push_back(motherNode);

		// Leaves by definition are not a dependency of any other node
		leaves.erase(std::remove(leaves.begin(), leaves.end(), motherNode), leaves.end());
	}
	if (fatherNode) {
		newNode->dependencies.push_back(fatherNode);

		leaves.erase(std::remove(leaves.begin(), leaves.end(), fatherNode), leaves.end());
	}

	if (needDependency.size() > 0) {
		for (auto node : needDependency) {
			node->dependencies.push_back(newNode);
		}
	} else {
		leaves.push_back(newNode);
	}

	mustUpdateCrossOrder = true;
	pedigreeSize++;
}


// TODO: std::set is faster and probably represents intent more closely
std::vector<std::shared_ptr<PedigreeNode>> Pedigree::depthFirstSearchManyNodes(bool(*predicate)(std::shared_ptr<PedigreeNode>)) {
	std::vector<std::shared_ptr<PedigreeNode>> returnNodes;

	std::function<
		void(std::shared_ptr<PedigreeNode>, bool(std::shared_ptr<PedigreeNode>), std::vector<std::shared_ptr<PedigreeNode>>*)
	> visitNode = [&visitNode](std::shared_ptr<PedigreeNode> node, bool(*predicate)(std::shared_ptr<PedigreeNode>), std::vector<std::shared_ptr<PedigreeNode>>* nodes) {
		if (predicate(node)) {
			// Don't want duplicates
			if (std::find(nodes->begin(), nodes->end(), node) == nodes->end()) nodes->push_back(node);
		}

		for (auto child : node->dependencies) {
			visitNode(child, predicate, nodes);
		}
	};

	for (auto leaf : leaves) {
		visitNode(leaf, predicate, &returnNodes);
	}

	return returnNodes;
}


std::vector<std::shared_ptr<PedigreeNode>> Pedigree::depthFirstSearchManyNodes(NodeChecker* predicate) {
	std::vector<std::shared_ptr<PedigreeNode>> returnNodes;

	std::function<void(std::shared_ptr<PedigreeNode>, NodeChecker*, std::vector<std::shared_ptr<PedigreeNode>>*)> visitNode = [&visitNode](std::shared_ptr<PedigreeNode> node, NodeChecker* predicate, std::vector<std::shared_ptr<PedigreeNode>>* nodes) {
		if ((*predicate)(node)) {
			if (std::find(nodes->begin(), nodes->end(), node) == nodes->end()) nodes->push_back(node);
		}

		for (auto child : node->dependencies) {
			visitNode(child, predicate, nodes);
		}
	};

	for (auto leaf : leaves) {
		visitNode(leaf, predicate, &returnNodes);
	}

	return returnNodes;
}


std::shared_ptr<PedigreeNode> Pedigree::depthFirstSearchSingleNode(NodeChecker* predicate) {
	std::shared_ptr<PedigreeNode> returnNode = nullptr;

	std::function<std::shared_ptr<PedigreeNode>(std::shared_ptr<PedigreeNode>, NodeChecker*)> visitNode = [&visitNode](std::shared_ptr<PedigreeNode> node, NodeChecker* predicate) {
		if ((*predicate)(node)) {
			return node;
		}

		for (auto child : node->dependencies) {
			auto deeperNode = visitNode(child, predicate);

			if (deeperNode != nullptr) {
				return deeperNode;
			}
		}

		return std::shared_ptr<PedigreeNode>();
	};

	for (auto leaf : leaves) {
		auto foundNode = visitNode(leaf, predicate);

		if (foundNode != nullptr) {
			returnNode = foundNode;
			break;
		}
	}

	return returnNode;
}


std::shared_ptr<PedigreeNode> Pedigree::depthFirstSearchSingleNode(bool(*predicate)(std::shared_ptr<PedigreeNode>)) {
	std::shared_ptr<PedigreeNode> returnNode = nullptr;
	
	std::function<
		std::shared_ptr<PedigreeNode>(std::shared_ptr<PedigreeNode>, bool(std::shared_ptr<PedigreeNode>))
	> visitNode = [&visitNode](std::shared_ptr<PedigreeNode> node, bool(*predicate)(std::shared_ptr<PedigreeNode>)) {
		if (predicate(node)) {
			return node;
		}

		for (auto child : node->dependencies) {
			auto deeperNode = visitNode(child, predicate);

			if (deeperNode != nullptr) {
				return deeperNode;
			}
		}

		return std::shared_ptr<PedigreeNode>();
	};

	for (auto leaf : leaves) {
		auto foundNode = visitNode(leaf, predicate);

		if (foundNode != nullptr) {
			returnNode = foundNode;
			break;
		}
	}

	return returnNode;
}


void Pedigree::updateCrossOrder() {
	crossOrder.clear();

	// Try to find a path through the pedigree
	// Use Kahn 1962 algorithm which works if pedigree is DAG (and it has to be if built correctly)

	// Construct list of all edges
	class EdgeGenerator : public NodeChecker {
	public:
		std::map<std::string, std::vector<std::string>> allEdges; // Maps from child to parent(s)

		bool operator()(std::shared_ptr<PedigreeNode> node) {
			// TODO: Slightly hacky - should clean this up

			// Account for the founders too
			bool mustAddMother = true;
			bool mustAddFather = true;

			// Add all edges
			for (auto& dependency : node->dependencies) {
				std::string from = node->cross.childName;
				std::string to = dependency->cross.childName;

				if (to == node->cross.motherName) mustAddMother = false;
				if (to == node->cross.fatherName) mustAddFather = false;

				// Should null-construct if none present
				auto existingEdges = allEdges[from];

				if (std::find(existingEdges.begin(), existingEdges.end(), to) == existingEdges.end()) {
					existingEdges.push_back(to);
					allEdges[from] = existingEdges;
				}
			}

			auto existing = allEdges[node->cross.childName];

			if (mustAddFather) {
				if (std::find(existing.begin(), existing.end(), node->cross.fatherName) == existing.end()) {
					existing.push_back(node->cross.fatherName);
				}
			}
			if (mustAddMother) {
				if (std::find(existing.begin(), existing.end(), node->cross.motherName) == existing.end()) {
					existing.push_back(node->cross.motherName);
				}
			}

			allEdges[node->cross.childName] = existing;

			return false;
		}
	} edgeGenerator;
	
	depthFirstSearchManyNodes(&edgeGenerator);

	auto& allEdges = edgeGenerator.allEdges;
	std::vector<std::shared_ptr<PedigreeNode>> nodesWithNoIncomingEdges = leaves;;
	std::vector<std::string> nodeNamesWithNoIncomingEdges;

	for (const auto& leaf : nodesWithNoIncomingEdges) {
		nodeNamesWithNoIncomingEdges.push_back(leaf->cross.childName);
	}

	while (nodesWithNoIncomingEdges.size() > 0) {
		auto nodeName = nodeNamesWithNoIncomingEdges.back();;
		auto node = nodesWithNoIncomingEdges.back();
		crossOrder.push_back(node);
		nodesWithNoIncomingEdges.pop_back();
		nodeNamesWithNoIncomingEdges.pop_back();

		auto connectedTo = allEdges[nodeName];

		while (connectedTo.size() > 0) {
			auto m = connectedTo.back();
			connectedTo.pop_back();

			bool mHasEdges = false;
			for (auto& entry : allEdges) {
				auto& vec = entry.second;

				if (entry.first != nodeName && std::find(vec.begin(), vec.end(), m) != vec.end()) {
					mHasEdges = true;
					break;
				}
			}

			if (!mHasEdges) {
				nodeNamesWithNoIncomingEdges.push_back(nodeName);

				if (node != nullptr) {
					for (auto& dependency : node->dependencies) {
						if (dependency->cross.childName == m) {
							nodesWithNoIncomingEdges.push_back(dependency);
							break;
						}
					}
				}
			}
		}

		allEdges[node->cross.childName] = std::vector<std::string>();
	}

	int numberOfEdgesLeft = 0;

	for (const auto& edgeSet : allEdges) {
		numberOfEdgesLeft += edgeSet.second.size();
	}

	if (numberOfEdgesLeft > 0) {
		throw std::runtime_error("Found cycle in pedigree.");
	}

	// Remove the nullptr (founders)
	crossOrder.erase(std::remove_if(crossOrder.begin(), crossOrder.end(), [](const std::shared_ptr<PedigreeNode>& ptr) {
		return ptr == nullptr;
	}), crossOrder.end());

	mustUpdateCrossOrder = false;
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
	return pedigree->crossOrder[index]->cross;
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
	return &pedigree->crossOrder[index]->cross;
}