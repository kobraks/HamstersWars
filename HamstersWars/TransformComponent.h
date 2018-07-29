#pragma once
#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>

#include "Entity.h"
#include "Component.h"
#include "Transformable.h"
namespace game::component
{
	class TransformComponent : public Component, public Transformable
	{
	public:
		TransformComponent(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& table);
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(TransformComponent&&) = default;

		TransformComponent& operator=(const TransformComponent&) = default;
		TransformComponent& operator=(TransformComponent&&) = default;

		virtual ~TransformComponent();

		Component* copy() const override;
		std::string get_name() const override;

	private:
		void parse_table(const LuaIntf::LuaRef& table);
	};

}
