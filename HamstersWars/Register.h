#pragma once
#include <Lua/Lua.hpp>
#include <Lua/LuaIntf.h>

namespace game::lua
{
	class Script;

	class Register
	{
	public:
		Register() = default;
		virtual ~Register() = 0
		{}

	protected:
		virtual void register_clas(LuaIntf::LuaContext& context) const = 0;

		friend class Script;
	};
}