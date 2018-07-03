#pragma once
#include <glm/glm.hpp>
#include "Rectangle.h"

namespace game
{
	class Glyph
	{
	public:
		Glyph();
		~Glyph();

		float advace;
		IntRect texture_rect;
		glm::vec4 bounds;
	};


}
