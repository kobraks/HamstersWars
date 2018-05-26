#pragma once
#include <functional>
#include <vector>
#include <string>

#include "Program.h"
#include "Model.h"

namespace game
{
	typedef std::function<void(gl::Program&, const model::Model*)> shader_behavior;

	class SceneManager
	{
	public:
		SceneManager();
		explicit SceneManager(const gl::Program& program, shader_behavior behavior = nullptr);
		~SceneManager();

		void draw();
		void update();

		void set_shader(const gl::Program& program);
		void set_shader_behavior(shader_behavior& behavior);

		void load_from_file(const std::string& file_name);

	private:
		shader_behavior shader_behavior_;
		std::vector<model::Model*> models_;
		gl::Program shader_;
	};
}

