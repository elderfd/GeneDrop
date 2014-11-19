#pragma once

#include <cassert>

//! Class emulating Haskell's Maybe type
template<class T> class Maybe
{
public:
	Maybe()
	{
		_set = false;
	}

	Maybe(T value)
	{
		setValue(value);
	}

	bool set() const
	{
		return _set;
	}

	T value() const
	{
		// Only allow access if the value has been set
		assert(_set);
		return _value;
	}

	void setValue(T value)
	{
		_value = value;
		_set = true;
	}

	explicit operator bool() const {
		return set();
	}
protected:
	bool _set;
	T _value;
};

