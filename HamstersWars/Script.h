#pragma once
#include <string>
#include <functional>
#include <unordered_map>

#include <Lua/Lua.hpp>
#include <Lua/LuaIntf.h>

namespace game
{
	class Script
	{
	public:
		Script() = default;
		~Script();

		operator LuaIntf::LuaContext&() const;
		static LuaIntf::LuaContext& lua();

		static void do_file(const std::string& file);
		static void do_string(const std::string& code);
		static LuaIntf::LuaBinding binding();

		static LuaIntf::LuaRef* lua_ref(const std::string& name);
	private:
		static LuaIntf::LuaContext lua_;
		static std::unordered_map<std::string, LuaIntf::LuaRef*> refs_;
	};
}

