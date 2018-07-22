#pragma once
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>

#include "Component.h"

namespace game::component
{
	class ColliderComponent : public Component
	{
	public:
		ColliderComponent(std::shared_ptr<Entity> owner, const LuaIntf::LuaRef& table);

		Component* copy() const override;
		~ColliderComponent();

	private:
		void parse_table(const LuaIntf::LuaRef& table);
	};
}

