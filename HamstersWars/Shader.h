#pragma once
#include <gl/glew.h>
#include <string>
#include <memory>

namespace gl
{
	namespace shader_type
	{
		enum shader_type_t
		{
			Vertex = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
			Geometry = GL_GEOMETRY_SHADER
		};
	}

	class Shader
	{
	public:
		Shader(shader_type::shader_type_t type);
		Shader(shader_type::shader_type_t type, const std::string& code);
		Shader(const Shader&) = default;

		Shader& operator=(const Shader&) = default;
		operator GLuint() const;

		void compile();
		void source(const std::string& code);
		void load_source_form_file(const std::string& path);

		std::string get_info_log() const;
	private:
		std::shared_ptr<GLuint> obj_;
	};
}