#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Light.h"

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
		void set_value(const Vector2D& value);
		void set_value(const Vector3D& value);
		void set_value(const glm::vec4& value);
		void set_value(const Texture& texture);
		//void set_value(const Light& light);

		ProgramParameter& operator=(const glm::mat4& value);
		ProgramParameter& operator=(const float& value);
		ProgramParameter& operator=(const int& value);
		ProgramParameter& operator=(const Vector2D& value);
		ProgramParameter& operator=(const Vector3D& value);
		ProgramParameter& operator=(const glm::vec4& value);
		ProgramParameter& operator=(const Texture& texture);
		//ProgramParameter& operator=(const Light& light);


		operator GLuint() const;
	private:
		GLuint param_id_;
		GLuint texture_id_;
		GLenum type_;
	};
}

