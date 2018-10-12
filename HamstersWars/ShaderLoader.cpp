#include "stdafx.h"
#include "ShaderLoader.h"
#include "Shader.h"
#include "Program.h"

namespace game
{
	gl::Shader* ShaderLoader::load_shader(const std::string& file, const TShader& type)
	{
		gl::Shader* shader = nullptr;

		switch (type)
		{
		case vertex:
			LOG(LOG_DEBUG, "Loading vertex shader form: %s", file.c_str());
			shader = new gl::Shader(gl::shader_type::Vertex);
			break;
		case fragment:
			LOG(LOG_DEBUG, "Loading fragment shader form: %s", file.c_str());
			shader = new gl::Shader(gl::shader_type::Fragment);
			break;
		case geometry:
			LOG(LOG_DEBUG, "Loading geometry shader form: %s", file.c_str());
			shader = new gl::Shader(gl::shader_type::Geometry);
			break;
		}

		try
		{
			LOG(LOG_DEBUG, "Loading shader from source file");
			shader->load_source_form_file(file);
			LOG(LOG_DEBUG, "Compiling shader");
			shader->compile();
		}
		catch (gl::exception::CompileException& ex)
		{
			LOG(LOG_ERROR, "Unable to compile shader: %s", ex.what());
			delete shader;
			return nullptr;
		}
		catch (std::exception& ex)
		{
			LOG(LOG_ERROR, "Unable to read shader source: %s", ex.what());
			delete shader;
			return nullptr;
		}

		return shader;
	}

	gl::Program* ShaderLoader::generate_program(gl::Shader* vertex, gl::Shader* fragment, gl::Shader* geometry)
	{
		auto program = new gl::Program();

		if (vertex)
		{
			LOG(LOG_DEBUG, "Attaching vertex shader");
			program->attach(*vertex);
		}
		else
			LOG(LOG_WARNING, "No vertex shader");

		if (fragment)
		{
			LOG(LOG_DEBUG, "Attaching fragment shader");
			program->attach(*fragment);
		}
		else
			LOG(LOG_WARNING, "No fragment shader");

		if (geometry)
		{
			LOG(LOG_DEBUG, "Attaching geometry shader");
			program->attach(*geometry);
		}
		else
			LOG(LOG_WARNING, "No geometry shader");

		try
		{
			LOG(LOG_DEBUG, "Linking the shader");
			program->link();
		}
		catch (gl::exception::LinkException& ex)
		{
			LOG(LOG_ERROR, "Unable to link shaders %s", ex.what());

			delete program;
			program = nullptr;
		}

		return program;
	}

	gl::Program* ShaderLoader::load_program(const std::string& file)
	{
		gl::Shader* vertex_shader = load_shader(file + ".vert", vertex);
		gl::Shader* fragment_shader = load_shader(file + ".frag", fragment);
		gl::Shader* geometry_shader = load_shader(file + ".geo", geometry);

		gl::Program* program = generate_program(vertex_shader, fragment_shader, geometry_shader);

		delete vertex_shader;
		delete fragment_shader;
		delete geometry_shader;

		return program;
	}
}
