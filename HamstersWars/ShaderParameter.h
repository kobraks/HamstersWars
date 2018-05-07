#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2d.h"

namespace gl
{
	class ShaderParameter
	{
	public:
		ShaderParameter();

		ShaderParameter(const GLuint& program_id, const GLchar* name, const GLenum& type);
		ShaderParameter(const GLuint& program_id, const GLchar* name, const GLenum& type, GLuint texture_id);

		void set_value(glm::mat4 value);
		void set_value(float value);
		void set_value(int value);
		void set_value(glm::vec2 value);
		void set_value(glm::vec3 value);
		void set_value(glm::vec4 value);
		void set_value(model::Texture2d& texture);

	private:
		GLuint param_id_;
		GLuint texture_id_;
		GLenum type_;
	};
}

