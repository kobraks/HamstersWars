#include "ProgramParameter.h"

#include "glm/gtc/type_ptr.hpp"

gl::ProgramParameter::ProgramParameter()
{
}


gl::ProgramParameter::ProgramParameter(const GLuint& program_id, const GLchar* name, const GLenum& type) : type_(type)
{	
	param_id_ = glGetUniformLocation(program_id, name);
	texture_id_ = -1;
}

gl::ProgramParameter::ProgramParameter(const GLuint& program_id, const GLchar* name, const GLenum& type,
	GLuint texture_id) : type_(type), texture_id_(texture_id)
{
	param_id_ = glGetUniformLocation(program_id, name);
}

void gl::ProgramParameter::set_value(const glm::mat4& value)
{
	glUniformMatrix4fv(param_id_, 1, GL_FALSE, glm::value_ptr(value));
}

void gl::ProgramParameter::set_value(const float& value)
{
	glUniform1f(param_id_, value);
}

void gl::ProgramParameter::set_value(const int& value)
{
	glUniform1i(param_id_, value);
}

void gl::ProgramParameter::set_value(const glm::vec2& value)
{
	glUniform2fv(param_id_, 1, glm::value_ptr(value));
}

void gl::ProgramParameter::set_value(const glm::vec3& value)
{
	glUniform3fv(param_id_, 1, glm::value_ptr(value));
}

void gl::ProgramParameter::set_value(const glm::vec4& value)
{
	glUniform4fv(param_id_, 1, glm::value_ptr(value));
}

void gl::ProgramParameter::set_value(const model::Texture2d& texture)
{
	glActiveTexture(GL_TEXTURE0 + texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(param_id_, texture_id_);
}

gl::ProgramParameter& gl::ProgramParameter::operator=(const glm::mat4& value)
{
	set_value(value);
	return *this;
}

gl::ProgramParameter& gl::ProgramParameter::operator=(const float& value)
{
	set_value(value);
	return *this;
}

gl::ProgramParameter& gl::ProgramParameter::operator=(const int& value)
{
	set_value(value);
	return *this;
}

gl::ProgramParameter& gl::ProgramParameter::operator=(const glm::vec2& value)
{
	set_value(value);
	return *this;
}

gl::ProgramParameter& gl::ProgramParameter::operator=(const glm::vec3& value)
{
	set_value(value);
	return *this;
}

gl::ProgramParameter& gl::ProgramParameter::operator=(const glm::vec4& value)
{
	set_value(value);
	return *this;
}

gl::ProgramParameter& gl::ProgramParameter::operator=(const model::Texture2d& texture)
{
	set_value(texture);
	return *this;
}

gl::ProgramParameter::operator unsigned() const
{
	return param_id_;
}
