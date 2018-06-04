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
#include "Types.h"

namespace game
{
	class SceneManager
	{
	public:
		SceneManager(const SceneManager&) = delete;

		SceneManager& operator = (const SceneManager&) = delete;

		~SceneManager();

		static void initialize(const gl::Program& program, shader_behavior behavior = nullptr);

		static void draw();
		static void update();

		static void set_shader(const gl::Program& program);
		static void set_shader_behavior(shader_behavior& behavior);

		static void load_from_file(const std::string& file_name);

		static std::vector<std::shared_ptr<Entity>> get_entites();

		static void destroy(std::shared_ptr<Entity> entity);

		static CollisionHandler* get_collision_handler();

		static void draw_bounding_boxes(bool enable);

	private:
		SceneManager();;
		static SceneManager * get_instance();

		shader_behavior shader_behavior_;
		std::vector<std::shared_ptr<Entity>> entities_;
		std::stack<std::shared_ptr<Entity>> to_destroy_;
		gl::Program shader_;
		bool draw_bounding_boxes_;

		void destroy();

		CollisionHandler* handler_;
	};
}

