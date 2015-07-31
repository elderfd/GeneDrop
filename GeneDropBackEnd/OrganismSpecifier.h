#pragma once
#include <string>


class Organism;


//! Defines a reference to a particular organism
/*!
	Organisms need to have a unique ID within a generation.
*/
class OrganismSpecifier {
	friend class State;
	friend struct std::hash<OrganismSpecifier>;
public:
	OrganismSpecifier();
	OrganismSpecifier(std::string ID, std::string generation);
	~OrganismSpecifier();

	bool operator==(const OrganismSpecifier& other) const;

	std::string name() const {
		return ID;
	}

	std::string generation() const {
		return _generation;
	}

	std::string displayString() const {
		return _generation + "::" + ID;
	}

	// For reading in from human input
	static OrganismSpecifier fromString(std::string inString);

	bool operator<(const OrganismSpecifier& other) const {
		return this->_generation < other._generation || this->_generation != other._generation && this->ID < other.ID;
	}

protected:
	bool isNull = true;
	std::string ID;
	std::string _generation;
};


// For storing in a hashtable
template<> struct std::hash<OrganismSpecifier> {
	std::size_t operator()(const OrganismSpecifier& k) const {
		return std::hash<std::string>()(k._generation) ^ (std::hash<std::string>()(k.ID) << 1);
	}
};
