#pragma once

#include "Maybe.h"
#include <string>
#include <vector>

class CommandLineArgInterface :
	public Maybe < std::string > {
public:
	//! Sets the value of the parameter attached to the argument
	virtual void setValueOfParameter() = 0;

	//! Whether the argument is ok or not (i.e. not missing if not optional)
	virtual Maybe<std::string> hasError() const = 0;

	//! Checks if the supplied key matches the key the parameter expects
	bool matchesKey(std::string key);

	//! Returns the key
	std::string getKey() const { return _key; }

protected:
	bool _optional;
	std::string _key;
};


template<class T> class CommandLineArg :
public CommandLineArgInterface {
public:
	CommandLineArg(std::string key, T* parameterToSet);
	CommandLineArg(std::string key, T* parameterToSet, T defaultValue);
	~CommandLineArg();

	void setValueOfParameter();

	Maybe<std::string> hasError() const;

protected:
	T defaultValue;
	T* parameterToSet;

	T value() const;
	T convertValue(std::string value) const;
};


template<class T> CommandLineArg<T>::CommandLineArg(std::string key, T* parameterToSet) {
	this->_optional = false;
	this->_key = key;
	this->parameterToSet = parameterToSet;
}


template<class T> CommandLineArg<T>::CommandLineArg(std::string key, T* parameterToSet, T defaultValue) {
	this->_optional = true;
	this->defaultValue = defaultValue;
	this->_key = key;
	this->parameterToSet = parameterToSet;
}


template<class T> CommandLineArg<T>::~CommandLineArg() {}


template<class T> void CommandLineArg<T>::setValueOfParameter() {
	*parameterToSet = value();
}


template<class T> T CommandLineArg<T>::value() const {
	T value;

	if (!set()) {
		assert(_optional);

		value = defaultValue;
	} else {
		value = convertValue(Maybe<std::string>::value());
	}

	return value;
}


template<class T> Maybe<std::string> CommandLineArg<T>::hasError() const {
	Maybe<std::string> errorMessage;

	if (!(set() || _optional)) {
		errorMessage.setValue("Required key \"" + _key + "\" not given value.");
	}

	return errorMessage;
}
