#pragma once
#include "GameException.h"

#include <GL/glew.h>

namespace game::exception
{
	class GlewException : public GameException
	{
	public:
		explicit GlewException(GLenum error_num) : GameException("%s", glewGetErrorString(error_num))
		{
		}
	};
}