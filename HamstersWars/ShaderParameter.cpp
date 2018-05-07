#include "ShaderParameter.h"

#include "glm/gtc/type_ptr.hpp"

gl::ShaderParameter::ShaderParameter()
{
}


gl::ShaderParameter::ShaderParameter(const GLuint& program_id, const GLchar* name, const GLenum& type) : type_(type)
{
	if (this == nullptr) { return; }
	param_id_ = glGetUniformLocation(program_id, name);
	texture_id_ = -1;
}

gl::ShaderParameter::ShaderParameter(const GLuint& program_id, const GLchar* name, const GLenum& type,
	GLuint texture_id) : type_(type), texture_id_(texture_id)
{
	if (this == nullptr) { return; }

	param_id_ = glGetUniformLocation(program_id, name);
}

void gl::ShaderParameter::set_value(glm::mat4 value)
{
	if (this == nullptr) { return; }

	glUniformMatrix4fv(param_id_, 1, GL_FALSE, glm::value_ptr(value));
}

void gl::ShaderParameter::set_value(float value)
{
	if (this == nullptr) { return; }

	glUniform1f(param_id_, value);
}

void gl::ShaderParameter::set_value(int value)
{
	if (this == nullptr) { return; }

	glUniform1i(param_id_, value);
}

void gl::ShaderParameter::set_value(glm::vec2 value)
{
	if (this == nullptr) { return; }

	glUniform2fv(param_id_, 1, glm::value_ptr(value));
}

void gl::ShaderParameter::set_value(glm::vec3 value)
{
	if (this == nullptr) { return; }

	glUniform3fv(param_id_, 1, glm::value_ptr(value));
}

void gl::ShaderParameter::set_value(glm::vec4 value)
{
	if (this == nullptr) { return; }

	glUniform4fv(param_id_, 1, glm::value_ptr(value));
}

void gl::ShaderParameter::set_value(model::Texture2d& texture)
{
	if (this == nullptr) { return; }

	glActiveTexture(GL_TEXTURE0 + texture_id_);
	texture.bind();

	glUniform1i(param_id_, texture_id_);
}
