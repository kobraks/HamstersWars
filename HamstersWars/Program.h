#pragma once
#include <gl/glew.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace model {
	class Texture2d;
}

namespace gl
{
	typedef GLint Uniform;
	typedef GLint Attribute;

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

		void set_attribute(const std::string& attrib_name, const Attribute& attribute);
		Attribute get_attribute(const std::string& name) const;

		void set_uniform(const Uniform& uniform, glm::mat4 value);
		void set_uniform(const Uniform& uniform, float value);
		void set_uniform(const Uniform& uniform, int value);
		void set_uniform(const Uniform& uniform, glm::vec2 value);
		void set_uniform(const Uniform& uniform, glm::vec3 value);
		void set_uniform(const Uniform& uniform, glm::vec4 value);
		void set_uniform(const Uniform& uniform, model::Texture2d& texture);

	private:
		std::shared_ptr<GLuint> obj_;
	};
}
