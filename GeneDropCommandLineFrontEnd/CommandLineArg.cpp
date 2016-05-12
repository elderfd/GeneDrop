#include "CommandLineArg.h"


bool CommandLineArgInterface::matchesKey(const std::string& key)
{
	bool matched = true;

	for (unsigned int j = 0; j < key.size(); j++)
	{
		if (toupper(key[j]) != toupper(_key[j]))
		{
			matched = false;
			break;
		}
	}

	return matched;
}


template<> int CommandLineArg<int>::convertValue(std::string value) const
{
	return std::stoi(value);
}


template<> double CommandLineArg<double>::convertValue(std::string value) const
{
	return std::stod(value);
}


template<> std::string CommandLineArg<std::string>::convertValue(std::string value) const
{
	return value;
}


template<> bool CommandLineArg<bool>::convertValue(std::string value) const
{
	auto firstChar = toupper(value[0]);

	if (firstChar == 'T')
	{
		return true;
	}
	else
	{
		// This relies on the input having been sanitised before
		assert(firstChar == 'F');

		return false;
	}
}