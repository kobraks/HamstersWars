#include "Program.h"
#include "LinkException.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "Log.h"

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
	else
		populate_parameters();
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

gl::Uniform gl::Program::get_uniform(const std::string& name) const
{
	return glGetUniformLocation(*obj_, name.c_str());
}

void gl::Program::set_uniform(const Uniform& uniform, const glm::mat4& value)
{
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}

void gl::Program::set_uniform(const Uniform& uniform, const float& value)
{
	glUniform1f(uniform, value);
}

void gl::Program::set_uniform(const Uniform& uniform, const int& value)
{
	glUniform1i(uniform, value);
}

void gl::Program::set_uniform(const Uniform& uniform, const Vector2D& value)
{
	glUniform2fv(uniform, 1, glm::value_ptr(glm::vec2(value)));
}

void gl::Program::set_uniform(const Uniform& uniform, const Vector3D& value)
{
	glUniform3fv(uniform, 1, glm::value_ptr(glm::vec3(value)));
}

void gl::Program::set_uniform(const Uniform& uniform, const glm::vec4& value)
{
	glUniform4fv(uniform, 1, glm::value_ptr(value));
}

void gl::Program::set_uniform(const Uniform& uniform, const GLuint& texture_id_, Texture& texture)
{
	glActiveTexture(GL_TEXTURE0 + texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(uniform, texture_id_);
}

void gl::Program::set_uniform(const std::string& name, const glm::mat4& value)
{
	get_parameter(name).set_value(value);
}

void gl::Program::set_uniform(const std::string& name, const float& value)
{
	get_parameter(name).set_value(value);
}

void gl::Program::set_uniform(const std::string& name, const Vector2D& value)
{
	get_parameter(name).set_value(value);
}

void gl::Program::set_uniform(const std::string& name, const Vector3D& value)
{
	get_parameter(name).set_value(value);
}

void gl::Program::set_uniform(const std::string& name, const glm::vec4& value)
{
	get_parameter(name).set_value(value);
}

void gl::Program::set_uniform(const std::string& name, const GLuint& texture_id, Texture& texture)
{
	get_parameter(name).set_value(texture);
}

gl::ProgramParameter gl::Program::operator[](const std::string& name) const
{
	return get_parameter(name);
}

gl::ProgramParameter gl::Program::operator[](const char* name) const
{
	return get_parameter(name);
}

gl::ProgramParameter gl::Program::get_parameter(const std::string& name) const
{
	auto iter = parameters_.find(name);

	if (iter != parameters_.end())
		return *iter->second;
	else
		throw std::exception();
}

void gl::Program::populate_parameters()
{
	GLint ucount = 0;
	glGetProgramiv(*obj_, GL_ACTIVE_UNIFORMS, &ucount);
	LOG(LOG_DEBUG, "Program attributes count: %i", ucount);

	GLchar buf[256];
	GLsizei u_length;
	GLsizei u_size;
	GLuint u_textureid = 0;
	GLenum u_type;

	for (int i = 0; i<ucount; i++)
	{
		glGetActiveUniform(*obj_, i, 256, &u_length, &u_size, &u_type, buf);
		LOG(LOG_DEBUG1, "Attribute: %i\n\tname: %s\n\ttype: %u\n\tlength: %u\n\tsize: %i", i, buf, u_type, u_length, u_size);
		std::string u_name(buf);

		if (u_type == GL_SAMPLER_2D)
		{

			parameters_[u_name] = std::make_shared<ProgramParameter>(*obj_, buf, u_type, u_textureid);
			u_textureid++;
		}
		else
			parameters_[u_name] = std::make_shared<ProgramParameter>(*obj_, buf, u_type);
	}
}
