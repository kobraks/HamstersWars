#pragma once
#include "Defines.h"
#include "GameException.h"

namespace game::exception
{
	class UnableToFindSpecifedTableKeyException : public GameException
	{
	public:
		UnableToFindSpecifedTableKeyException(const std::string& key) : GameException(UNABLE_TO_FIND_SPECIFED_TABLE_KEY_EXCEPTION, key.c_str())
		{
			
		}
	};
}