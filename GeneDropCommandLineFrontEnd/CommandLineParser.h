#pragma once

#include <string>
#include <vector>
#include <assert.h>

class CommandLineParser
{
public:
	CommandLineParser(int argc, char *argv[]);
	~CommandLineParser();

	class SuccessfulParseResults
	{
		std::string pedigreeFileName;
	};

protected:
	//! Class emulating Haskell's Maybe type
	template<class T> class Maybe
	{
	public:
		Maybe()
		{
			_set = false;
		}

		bool set() const
		{
			return _set;
		}

		T value() const
		{
			// Only allow access if the value has been set
			assert(hasValue);
			return _value;
		}

		void setValue(T value)
		{
			_value = value;
		}

		explicit operator bool() const {
			return set();
		}
	protected:
		bool _set;
		T _value;
	};
	
	// Parses the input and returns true if successful
	bool parse(int argc, char *argv[]);

	// Finds the value matching a key from the input
	Maybe<std::string> getValueForKey(std::string key, int argc, char *argv[]);

	// Returns if toCheck unambiguously matches key or not
	bool unambigiousKeyMatch(std::string toCheck, std::string key);

	static const std::string
		pedigreeFileKey,
		genotypeFileKey,
		lociFileKey,
		settingsFileKey;

	static const std::vector<std::string> allKeys;
};

