#pragma once
#include <gl/glew.h>
#include <glm/vec3.hpp>

namespace gl
{
	namespace light_param
	{
		enum light_param_t
		{
			Ambient = GL_AMBIENT,
			Diffuse = GL_DIFFUSE,
			Specular = GL_SPECULAR,
			Position = GL_POSITION,

			Constant = GL_CONSTANT_ATTENUATION,
			Linear = GL_LINEAR_ATTENUATION,
			Quadratic = GL_QUADRATIC_ATTENUATION
		};
	}

	class Light
	{
	public:
		explicit Light(const short& light = 0);



	private:
		glm::vec3 ambient_;
		glm::vec3 diffuse_;
		glm::vec3 specular_;
		glm::vec3 position_;

		float constant_ = 0;
		float linear_ = 0;
		float quadratic_ = 0;
	};
}
