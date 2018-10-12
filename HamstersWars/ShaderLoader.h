#pragma once

namespace gl
{
	class Shader;
	class Program;
}

namespace game
{
	class ShaderLoader
	{
	public:
		enum TShader : int { fragment = 0, geometry, vertex };

		static gl::Shader* load_shader(const std::string& file, const TShader& type);
		static gl::Program* generate_program(gl::Shader* vertex, gl::Shader* fragment, gl::Shader* geometry = nullptr);
		static gl::Program* load_program(const std::string& file);
	};
}

