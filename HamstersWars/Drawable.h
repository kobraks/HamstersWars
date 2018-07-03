#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "Program.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Types.h"
#include "Material.h"
#include "Transform.h"


namespace game
{
	class Drawable
	{
	public:
		Drawable() = default;

		virtual void draw(game::Transform& transofrm) = 0;
		virtual ~Drawable() = default;
	};
}
