#pragma once
#include <gl/glew.h>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>

#include "ProgramParameter.h"
#include "Light.h"
#include "Vector3D.h"
#include "Vector3D.h"
#include "Texture.h"

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
		void set_uniform(const Uniform& uniform, const Vector2D& value);
		void set_uniform(const Uniform& uniform, const Vector3D& value);
		void set_uniform(const Uniform& uniform, const glm::vec4& value);
		void set_uniform(const Uniform& uniform, const GLuint& texture_id_, Texture& texture);
		void set_uniform(const Uniform& uniform, const Light& light);

		void set_uniform(const std::string& name, const glm::mat4& value);
		void set_uniform(const std::string& name, const float& value);
		void set_uniform(const std::string& name, const Vector2D& value);
		void set_uniform(const std::string& name, const Vector3D& value);
		void set_uniform(const std::string& name, const glm::vec4& value);
		void set_uniform(const std::string& name, const GLuint& texture_id, Texture& texture);
		void set_uniform(const std::string& name, const Light& light);

		ProgramParameter operator[] (const std::string& name) const;
		ProgramParameter operator[] (const char* name) const;
		ProgramParameter get_parameter(const std::string& name) const;
	private:
		std::shared_ptr<GLuint> obj_;

		std::unordered_map<std::string, std::shared_ptr<ProgramParameter>> parameters_;

		void populate_parameters();
	};
}
