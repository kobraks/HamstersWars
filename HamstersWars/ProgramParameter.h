#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture2d.h"

namespace gl
{
	class ProgramParameter
	{
	public:
		ProgramParameter();

		ProgramParameter(const GLuint& program_id, const GLchar* name, const GLenum& type);
		ProgramParameter(const GLuint& program_id, const GLchar* name, const GLenum& type, GLuint texture_id);

		void set_value(const glm::mat4& value);
		void set_value(const float& value);
		void set_value(const int& value);
		void set_value(const glm::vec2& value);
		void set_value(const glm::vec3& value);
		void set_value(const glm::vec4& value);
		void set_value(const model::Texture2d& texture);

		ProgramParameter& operator=(const glm::mat4& value);
		ProgramParameter& operator=(const float& value);
		ProgramParameter& operator=(const int& value);
		ProgramParameter& operator=(const glm::vec2& value);
		ProgramParameter& operator=(const glm::vec3& value);
		ProgramParameter& operator=(const glm::vec4& value);
		ProgramParameter& operator=(const model::Texture2d& texture);


		operator GLuint() const;
	private:
		GLuint param_id_;
		GLuint texture_id_;
		GLenum type_;
	};
}

