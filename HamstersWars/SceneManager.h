#pragma once
#include <functional>
#include <vector>
#include <stack>
#include <string>
#include <memory>

#include "Program.h"
#include "Model.h"
#include "Entity.h"
#include "CollisionHandler.h"

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

		std::vector<std::shared_ptr<Entity>> get_entites() const;

		void destroy(std::shared_ptr<Entity> entity);

	private:
		shader_behavior shader_behavior_;
		std::vector<std::shared_ptr<Entity>> entities_;
		std::stack<std::shared_ptr<Entity>> to_destroy_;
		gl::Program shader_;

		void destroy();

		CollisionHandler* handler_;
	};
}

