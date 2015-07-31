#pragma once
#include "Cross.h"
#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
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


//! Describes a set of crosses to make
class Pedigree {
	friend class CrossIterator;

public:
	Pedigree();
	~Pedigree();

	CrossIterator begin();
	CrossIterator end() const;

	void addCross(const OrganismSpecifier& father, const OrganismSpecifier& mother, const OrganismSpecifier& child);

	unsigned int size() const { return pedigreeSize;  }

private:
	int pedigreeSize = 0;

	void updateCrossOrder();
	bool mustUpdateCrossOrder = true;
	
	std::vector<Cross> crossOrder;
	std::unordered_set<OrganismSpecifier> nodes;
	std::unordered_map<OrganismSpecifier, std::unordered_set<OrganismSpecifier>> edges; // Key = child, Value = parents
};
