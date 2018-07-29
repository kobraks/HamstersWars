#pragma once
#include "GameException.h"
#include "Defines.h"

namespace game::exception
{
	class NotFunctionExcpetion : public GameException
	{
	public:
		NotFunctionExcpetion(const std::string& function_name) : GameException(NOT_FUNCTION_EXCEPTION) 
		{  }
	};
}