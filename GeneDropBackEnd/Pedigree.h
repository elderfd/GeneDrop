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

	void prepareForUse();

	void addCross(const OrganismSpecifier& father, const OrganismSpecifier& mother, const OrganismSpecifier& child);

	unsigned int size() const { return pedigreeSize;  }

	std::vector<std::string> getGenerationsContainingName(const std::string& name) const;
	std::vector<std::string> getAllGenerations() const;

private:
	int pedigreeSize = 0;

	void updateCrossOrder();
	bool mustUpdateCrossOrder = true;
	
	// Can't rely on hash collisions due to weak equality
	static void addNode(std::unordered_set<OrganismSpecifier>& set, const OrganismSpecifier& newNode);
	static std::unordered_set<OrganismSpecifier>& getSet(std::unordered_map<OrganismSpecifier, std::unordered_set<OrganismSpecifier>>& map, OrganismSpecifier setSpecifier);

	std::vector<Cross> crossOrder;
	std::unordered_set<OrganismSpecifier> nodes;
	std::unordered_map<OrganismSpecifier, std::unordered_set<OrganismSpecifier>> edges; // Key = child, Value = parents
};
