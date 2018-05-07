#pragma once
#include <gl/glew.h>
#include <string>
#include <memory>

namespace gl
{
	class Shader;


	class Program
	{
	public:
		Program();
		Program(const Program&) = default;
		explicit Program(Shader& vertex_shader);
		Program(Shader& vertex_shader, Shader& fragment_shader);
		Program(Shader& vertex_shader, Shader& fragment_shader, Shader& geometry_shader);

		operator GLuint() const;

		void attach(Shader& shader);
		void link();

		void use();

		std::string get_info_log() const;

	private:
		std::shared_ptr<GLuint> obj_;
	};
}
