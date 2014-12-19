#pragma once
#include "Maybe.h"
#include <string>

template<class T> class CommandLineArg :
	public Maybe<std::string>
{
public:
	CommandLineArg(std::string key, bool optional = false, T defaultValue = T());
	~CommandLineArg();

	//! Extract the value
	T value() const;
	
	//! Whether the argument is ok or not (i.e. not missing if not optional)
	Maybe<std::string> hasError() const;

protected:
	T defaultValue;
	bool _optional;

	T convertValue(std::string value) const;
};


template<class T> CommandLineArg<T>::CommandLineArg(bool optional = false, T defaultValue = T())
{
	this->_optional = optional;
	this->defaultValue = defaultValue;
}


template<class T> CommandLineArg<T>::~CommandLineArg()
{
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


template<class T> T CommandLineArg<T>::value() const
{
	T value;

	if (!set())
	{
		assert(!_optional);

		value = defaultValue;
	}
	else
	{
		value = convertValue(Maybe<std::string>::value());
	}

	return value;
}


template<class T> Maybe<std::string> CommandLineArg<T>::hasError() const
{
	Maybe<std::string> errorMessage;

	if (!(set() || _optional))
	{
		return errorMessage.setValue("Required parameter not given value.");
	}

	return errorMessage;
}