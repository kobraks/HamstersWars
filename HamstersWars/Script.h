#pragma once
#include <string>

#include <Lua/Lua.hpp>
#include <Lua/LuaIntf.h>

namespace game
{
	class Script
	{
	public:
		Script() = default;

		operator LuaIntf::LuaContext&() const;
		static LuaIntf::LuaContext& lua();

		static void do_file(const std::string& file);
		static LuaIntf::LuaBinding binding();
	private:
		static LuaIntf::LuaContext lua_;
	};
}

