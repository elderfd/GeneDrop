#include "CommandLineParser.h"
#include <initializer_list>
#include "Maybe.h"
#include <fstream>


CommandLineParser::CommandLineParser(int argc, char *argv[]) {
	parse(argc, argv);
}


CommandLineParser::~CommandLineParser() {
	while (expectedArgs.size() > 0) {
		delete expectedArgs[expectedArgs.size() - 1];
		expectedArgs.pop_back();
	}
}


void CommandLineParser::parse(int argc, char *argv[]) {
	// Try to match all keys to something
	for (int argIndex = 0; argIndex < argc; argIndex++) {
		// See if it's a key
		if (argv[argIndex][0] == '-') {
			// Strip off the dash
			auto trimmed = std::string(argv[argIndex]);
			trimmed.erase(trimmed.begin());

			bool takesValue = true;

			// Check if it takes options
			if (trimmed[0] == '-') {
				takesValue = false;
				trimmed.erase(trimmed.begin());
			}

			// Then check if it matches anything (checking we're not at the end of the input first)
			if (argIndex != (takesValue ? argc - 1 : argc)) {
				if (auto argMatch = getUnambiguousKeyMatch(trimmed)) {
					if (takesValue) {
						argIndex++;
						argMatch.value()->setValue(argv[argIndex]);
					} else {
						argMatch.value()->setValue("T");
					}
				}
			}
		}
	}

	// Check that everything is ok
	for (auto& arg : expectedArgs) {
		if (auto errorMessage = arg->hasError()) {
			errorStack.push_back(errorMessage.value());
		}
	}
}


Maybe<CommandLineArgInterface*> CommandLineParser::getUnambiguousKeyMatch(std::string key) {
	std::vector<CommandLineArgInterface*> foundArgs;
	Maybe<CommandLineArgInterface*> returnObj;

	for (auto arg : expectedArgs) {
		if (arg->matchesKey(key)) {
			foundArgs.push_back(arg);
		}
	}

	// If too many found or none found make an error
	if (foundArgs.size() > 1 || foundArgs.size() == 0) {
		std::string errorMessage = "No unambigious matches for key -" + key + " found; ";

		if (foundArgs.size() > 1) {
			errorMessage += "possible matches are: ";
			bool firstDone = false;

			for (auto& arg : foundArgs) {
				if (!firstDone) {
					firstDone = true;
				} else {
					errorMessage += ",";
				}
				errorMessage += arg->getKey();
			}

			errorMessage += ".";
		} else {
			errorMessage += "no matches found.";
		}

		warningStack.push_back(errorMessage);

		return returnObj;
	}

	// Unambiguous match found so return it
	returnObj.setValue(foundArgs[0]);

	return returnObj;
}


Maybe<std::vector<std::string>> CommandLineParser::errorsEncountered() {
	Maybe<std::vector<std::string>> retVal;

	if (errorStack.size() > 0) {
		retVal.setValue(errorStack);
	}

	return retVal;
}


Maybe<std::vector<std::string>> CommandLineParser::warningsEncountered() {
	Maybe<std::vector<std::string>> retVal;

	if (warningStack.size() > 0) {
		retVal.setValue(warningStack);
	}

	return retVal;
}


void CommandLineParser::setAllValues() {
	for (auto& arg : expectedArgs) {
		arg->setValueOfParameter();
	}
}


void CommandLineParser::setSingleValue(const std::string& key) {
	for (auto& arg : expectedArgs) {
		if (key == arg->getKey()) {
			arg->setValueOfParameter();
		}
	}
}