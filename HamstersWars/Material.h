#pragma once
#include <glm/glm.hpp>

#include <memory>
#include <assimp/color4.h>
#include "Texture.h"
#include "Color.h"

namespace game::model
{
	class Material
	{
	public:
		Material() = default;


		gl::Color ambient = glm::vec4(0);
		gl::Color diffuse = glm::vec4(0);
		gl::Color specular = glm::vec4(0);
		float shininess = 0.f;
		std::shared_ptr<gl::Texture> texture = nullptr;
	};
}

