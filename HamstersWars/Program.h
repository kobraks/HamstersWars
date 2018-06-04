#pragma once
#include <gl/glew.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <map>

#include "ProgramParameter.h"

namespace model {
	class Texture2d;
}

namespace gl
{
	typedef GLuint Uniform;
	typedef GLuint Attribute;

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
		Uniform get_uniform(const std::string& name) const;

		void set_uniform(const Uniform& uniform, const glm::mat4& value);
		void set_uniform(const Uniform& uniform, const float& value);
		void set_uniform(const Uniform& uniform, const int& value);
		void set_uniform(const Uniform& uniform, const glm::vec2& value);
		void set_uniform(const Uniform& uniform, const glm::vec3& value);
		void set_uniform(const Uniform& uniform, const glm::vec4& value);
		void set_uniform(const Uniform& uniform, const GLuint& texture_id_, model::Texture2d& texture);

		void set_uniform(const std::string& name, const glm::mat4& value);
		void set_uniform(const std::string& name, const float& value);
		void set_uniform(const std::string& name, const glm::vec2& value);
		void set_uniform(const std::string& name, const glm::vec3& value);
		void set_uniform(const std::string& name, const glm::vec4& value);
		void set_uniform(const std::string& name, const GLuint& texture_id, model::Texture2d& texture);

		std::shared_ptr<ProgramParameter> operator[] (const std::string& name);
		std::shared_ptr<ProgramParameter> get_parameter(const std::string& name);
	private:
		std::shared_ptr<GLuint> obj_;

		std::map<std::string, std::shared_ptr<ProgramParameter>> parameters_;

		void populate_parameters();
	};
}
