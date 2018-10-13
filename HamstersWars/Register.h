#pragma once

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
		virtual void register_class(LuaIntf::LuaBinding& binding) const = 0;

		friend class Script;
	};
}