#pragma once
#include "Cross.h"
#include <map>
#include <memory>
#include <vector>

class NewPedigree;

class CrossIterator {
public:
	CrossIterator();
	CrossIterator(const NewPedigree* pedigree);
	CrossIterator& operator++();
	CrossIterator operator++(int);
	const Cross& operator*();
	bool operator==(const CrossIterator& other);
	bool operator!=(const CrossIterator& other);
	const Cross* operator->();

private:
	const NewPedigree* pedigree = nullptr;
	unsigned int index = 0;
};


class NewPedigreeNode {
public:
	NewPedigreeNode() {};

	Cross cross;

	std::vector<std::shared_ptr<NewPedigreeNode>> dependencies;
};

// TODO: Move to better network representation for easier manipulation

//! Describes a set of crosses to make
class NewPedigree {
	friend class CrossIterator;

public:
	NewPedigree();
	~NewPedigree();

	CrossIterator begin();
	CrossIterator end() const;

	void addCross(std::string fatherName, std::string motherName, std::string childName);

	unsigned int size() const { return pedigreeSize;  }

private:
	// Leaves of the pedigree - no other crosses have dependencies on these
	std::vector<std::shared_ptr<NewPedigreeNode>> leaves;

	std::vector<std::shared_ptr<NewPedigreeNode>> depthFirstSearchManyNodes(bool(*predicate)(std::shared_ptr<NewPedigreeNode>));
	std::shared_ptr<NewPedigreeNode> depthFirstSearchSingleNode(bool(*predicate)(std::shared_ptr<NewPedigreeNode>));

	class NodeChecker {
	public:
		virtual bool operator()(std::shared_ptr<NewPedigreeNode> node) = 0;
	};

	std::vector<std::shared_ptr<NewPedigreeNode>> depthFirstSearchManyNodes(NodeChecker* predicate);
	std::shared_ptr<NewPedigreeNode> depthFirstSearchSingleNode(NodeChecker* predicate);

	int pedigreeSize = 0;

	void updateCrossOrder();
	bool mustUpdateCrossOrder = true;
	std::vector<std::shared_ptr<NewPedigreeNode>> crossOrder;
};
