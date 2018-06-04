#pragma once
#include <glm/glm.hpp>

#include <memory>
#include "Texture2d.h"
#include <assimp/color4.h>

namespace model
{
	class Material
	{
	public:
		Material() = default;
		Material(const aiColor4D& color, const float& specular_intensity, const float& specular_power, std::shared_ptr<Texture2d> texture);
		Material(const glm::vec4& color, const float& specular_intensity, const float& specular_power, std::shared_ptr<Texture2d> texture);

		glm::vec4 color = glm::vec4(1, 1, 1, 1);
		float specular_intensity = 0;
		float specular_power = 0;
		std::shared_ptr<Texture2d> texture = nullptr;
	};
}

