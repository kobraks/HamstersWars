#pragma once
#include <glm/glm.hpp>

#include "Texture2d.h"
#include <assimp/color4.h>

namespace model
{
	class Material
	{
	public:
		Material();
		Material(aiColor4D color, float specular_intensity, float specular_power, Texture2d* texture);
		~Material();

		glm::vec3 color;
		float specular_intensity;
		float specular_power;
		Texture2d* texture;

	};
}

