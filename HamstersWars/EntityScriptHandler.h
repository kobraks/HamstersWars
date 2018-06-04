#pragma once
#include "Entity.h"

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include <glm/vec3.hpp>
#include <memory>

namespace game::script
{
	class EntityScriptHandler
	{
	public:
		EntityScriptHandler(std::shared_ptr<Entity> entity);
		~EntityScriptHandler();

		float get_elapsed_time();

		void rotate(float angle, const glm::vec3& axis);
		void translate(const glm::vec3& axis);
		void scale(const glm::vec3& axis);

		void destroy();

		bool is_colliding();

		std::vector<EntityScriptHandler> get_colliders();

		void set_texture(const std::string& file);
		void set_model(const std::string& file);

		void print_log(const std::string& log_level, const std::string& log);

		void print_error(const std::string& error_message);

	private:
		std::shared_ptr<Entity> entity_;
	};
}
