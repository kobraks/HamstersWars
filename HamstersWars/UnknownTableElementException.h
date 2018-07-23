#pragma once
#include "Defines.h"
#include "GameException.h"

namespace game::exception
{
	class UnknownTableElementException : public GameException
	{
	public:
		UnknownTableElementException() : GameException(UNKNOWN_TABLE_ELEMENT_EXCEPTION)
		{}
	};
}