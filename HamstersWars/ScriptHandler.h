#pragma once
#include <memory>
#include <string>

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include "Component.h"

namespace game::component
{
	class ScriptHandler : public Component
	{
	public:
		explicit ScriptHandler(Entity* owner, const std::string& code);
		ScriptHandler(Entity* owner, const std::shared_ptr<LuaIntf::LuaContext>& context, const LuaIntf::LuaRef& function);
		~ScriptHandler();

		Component* copy();

		void update();
	private:
		std::shared_ptr<LuaIntf::LuaContext> context_;
		LuaIntf::LuaRef function_;

		void register_functions();
	};

}
