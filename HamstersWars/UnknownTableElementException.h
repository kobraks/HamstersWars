#pragma once
#include <exception>
#include "Defines.h"

namespace game::exception
{
	class UnknownTableElementException : public std::exception
	{
	public:
		UnknownTableElementException()
		{}

		char const* what() const override
		{
			return UNKNOWN_TABLE_ELEMENT_EXCEPTION;
		}
	};
}