#include "stdafx.h"
#include "Shader.h"
#include "Log.h"

gl::Shader::Shader(shader_type::shader_type_t type)
{
	obj_ = std::shared_ptr<GLuint>(new GLuint(0), [=](GLuint* id) { glDeleteShader(*id); });

	*obj_ = glCreateShader(type);
}

gl::Shader::Shader(shader_type::shader_type_t type, const std::string& code) : Shader(type)
{
	source(code);
	compile();
}

gl::Shader::Shader(shader_type::shader_type_t type, std::istream& stream)
{
	std::stringstream str;

	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	str << stream.rdbuf();

	source(str.str());
}

gl::Shader::operator unsigned() const
{
	return *obj_;
}

void gl::Shader::compile()
{
	GLint compiled;

	glCompileShader(*obj_);
	glGetShaderiv(*obj_, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
		throw exception::CompileException(get_info_log());
}

void gl::Shader::source(const std::string& code)
{
	const char* c = code.c_str();

	glShaderSource(*obj_, 1, &c, NULL);
}

void gl::Shader::load_source_form_file(const std::string& path)
{
	std::ifstream file_stream;
	std::stringstream stream;
	
	file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file_stream.open(path);

	stream << file_stream.rdbuf();

	file_stream.close();
	source(stream.str());
}

std::string gl::Shader::get_info_log() const
{
	GLint lenght;
	glGetShaderiv(*obj_, GL_INFO_LOG_LENGTH, &lenght);

	if (lenght > 0)
	{
		std::string info_log(lenght, 0);
		glGetShaderInfoLog(*obj_, lenght, &lenght, &info_log[0]);
		return info_log;
	}
	else
		return "";
}
