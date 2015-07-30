#pragma once
#include "Cross.h"
#include <map>
#include <memory>
#include <vector>

class Pedigree;

class CrossIterator {
public:
	CrossIterator();
	CrossIterator(const Pedigree* pedigree);
	CrossIterator& operator++();
	CrossIterator operator++(int);
	const Cross& operator*();
	bool operator==(const CrossIterator& other);
	bool operator!=(const CrossIterator& other);
	const Cross* operator->();

private:
	const Pedigree* pedigree = nullptr;
	unsigned int index = 0;
};


class PedigreeNode {
public:
	PedigreeNode() {};

	Cross cross;

	std::vector<std::shared_ptr<PedigreeNode>> dependencies;
};

// TODO: Move to better network representation for easier manipulation

//! Describes a set of crosses to make
class Pedigree {
	friend class CrossIterator;

public:
	Pedigree();
	~Pedigree();

	CrossIterator begin();
	CrossIterator end() const;

	void addCross(std::string fatherName, std::string motherName, std::string childName);

	unsigned int size() const { return pedigreeSize;  }

private:
	// Leaves of the pedigree - no other crosses have dependencies on these
	std::vector<std::shared_ptr<PedigreeNode>> leaves;

	std::vector<std::shared_ptr<PedigreeNode>> depthFirstSearchManyNodes(bool(*predicate)(std::shared_ptr<PedigreeNode>));
	std::shared_ptr<PedigreeNode> depthFirstSearchSingleNode(bool(*predicate)(std::shared_ptr<PedigreeNode>));

	class NodeChecker {
	public:
		virtual bool operator()(std::shared_ptr<PedigreeNode> node) = 0;
	};

	std::vector<std::shared_ptr<PedigreeNode>> depthFirstSearchManyNodes(NodeChecker* predicate);
	std::shared_ptr<PedigreeNode> depthFirstSearchSingleNode(NodeChecker* predicate);

	int pedigreeSize = 0;

	void updateCrossOrder();
	bool mustUpdateCrossOrder = true;
	std::vector<std::shared_ptr<PedigreeNode>> crossOrder;
};
