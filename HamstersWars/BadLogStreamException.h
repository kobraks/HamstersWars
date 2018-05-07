#pragma once
#include "GameException.h"
#include "Defines.h"

namespace game::exception
{
	class BadLogStreamException : public GameException
	{
	public:
		BadLogStreamException() : GameException(BAD_LOG_STREAM_EXCEPTION)
		{
			
		}
	};
}