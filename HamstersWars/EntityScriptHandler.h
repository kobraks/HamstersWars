#pragma once
#include "Entity.h"

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include <glm/vec3.hpp>

namespace game::script
{
	class EntityScriptHandler
	{
	public:
		EntityScriptHandler(Entity* entity);
		~EntityScriptHandler();

		float get_elapsed_time();

		//void scale(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);
		void translate(float x, float y, float z);
		void scale(glm::vec3);

		void destroy();


	};
}
