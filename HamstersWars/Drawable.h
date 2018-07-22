#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "Program.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Types.h"
#include "Material.h"
#include "Transform.h"
#include "Program.h"


namespace game
{
	class Drawable
	{
	public:
		Drawable() = default;

		virtual void draw(gl::Program& program, game::Transform& transofrm) = 0;
		virtual ~Drawable() = default;
	};
}
