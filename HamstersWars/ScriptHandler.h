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
		ScriptHandler(std::shared_ptr<Entity> owner, const std::string& code);
		explicit ScriptHandler(std::shared_ptr<Entity> owner);
		ScriptHandler(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& component_table);

		Component* copy() const override;

		void update();
	private:
		LuaIntf::LuaRef function_;

		static void register_functions(LuaIntf::LuaContext& context);
		static bool registered_;
	};

}
