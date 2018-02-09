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
		std::string forDisplay = _generation;

		if (!ID.empty()) {
			if (!_generation.empty()) forDisplay += "::";
			forDisplay += ID;
		}
		
		return forDisplay;
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
// Equal specifiers have to give the same hash
template<> struct std::hash<OrganismSpecifier> {
	std::size_t operator()(const OrganismSpecifier& k) const {
		return std::hash<std::string>()(k._generation) ^ (std::hash<std::string>()(k.ID) << 1);
	}
};


template<> struct std::equal_to <OrganismSpecifier> {
	bool operator()(const OrganismSpecifier& lhs, const OrganismSpecifier& rhs) const {
		return lhs == rhs;
	}
};