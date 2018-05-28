#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include "Component.h"

namespace LuaIntf
{
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::map)
}

namespace game::component
{
	class ScriptHandler : public Component
	{
	public:
		explicit ScriptHandler(std::shared_ptr<Entity> owner, const std::string& code);
		ScriptHandler(std::shared_ptr<Entity> owner, const std::shared_ptr<LuaIntf::LuaContext>& context, const LuaIntf::LuaRef& function);
		~ScriptHandler();

		Component* copy();

		void update();
	private:
		std::shared_ptr<LuaIntf::LuaContext> context_;
		LuaIntf::LuaRef function_;

		void register_functions();
	};

}
