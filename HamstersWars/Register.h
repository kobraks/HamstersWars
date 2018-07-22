#pragma once

namespace game::lua
{
	class Register
	{
	public:
		Register() = default;

		virtual void reg() const = 0;

		virtual ~Register() = 0
		{
		}
	};
}