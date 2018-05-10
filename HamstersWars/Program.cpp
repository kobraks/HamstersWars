#include "Program.h"
#include "LinkException.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Texture2d.h"

gl::Program::Program()
{
	obj_ = std::shared_ptr<GLuint>(new GLuint(0), [=](GLuint* id) { glDeleteProgram(*id); });
	*obj_ = glCreateProgram();
}

gl::Program::Program(Shader& vertex_shader) : Program()
{
	attach(vertex_shader);
	link();
}

gl::Program::Program(Shader& vertex_shader, Shader& fragment_shader) : Program()
{
	attach(vertex_shader);
	attach(fragment_shader);
	link();
}

gl::Program::Program(Shader& vertex_shader, Shader& fragment_shader, Shader& geometry_shader) : Program()
{
	attach(vertex_shader);
	attach(fragment_shader);
	attach(geometry_shader);
	link();
}

gl::Program::operator unsigned() const
{
	return *obj_;
}

void gl::Program::attach(Shader& shader)
{
	glAttachShader(*obj_, shader);
}

void gl::Program::link()
{
	GLint linked;

	glLinkProgram(*obj_);
	glGetProgramiv(*obj_, GL_LINK_STATUS, &linked);

	if (linked == GL_FALSE)
		throw exception::LinkException(get_info_log());
}

void gl::Program::use()
{
	glUseProgram(*obj_);
}

std::string gl::Program::get_info_log() const
{
	GLint lenght;
	glGetProgramiv(*obj_, GL_INFO_LOG_LENGTH, &lenght);

	if (lenght > 0)
	{
		std::string info_log(lenght, 0);
		glGetProgramInfoLog(*obj_, lenght, &lenght, &info_log[0]);
		return info_log;
	}
	else
		return "";
}

void gl::Program::set_attribute(const std::string& attrib_name, const Attribute& attribute)
{
	glBindAttribLocation(*obj_, attribute, attrib_name.c_str());
}

gl::Attribute gl::Program::get_attribute(const std::string& name) const
{
	return glGetAttribLocation(*obj_, name.c_str());
}

void gl::Program::set_uniform(const Uniform& uniform, glm::mat4 value)
{
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}

void gl::Program::set_uniform(const Uniform& uniform, float value)
{
	glUniform1f(uniform, value);
}

void gl::Program::set_uniform(const Uniform& uniform, int value)
{
	glUniform1i(uniform, value);
}

void gl::Program::set_uniform(const Uniform& uniform, glm::vec2 value)
{
	glUniform2fv(uniform, 1, glm::value_ptr(value));
}

void gl::Program::set_uniform(const Uniform& uniform, glm::vec3 value)
{
	glUniform3fv(uniform, 1, glm::value_ptr(value));
}

void gl::Program::set_uniform(const Uniform& uniform, glm::vec4 value)
{
	glUniform4fv(uniform, 1, glm::value_ptr(value));
}