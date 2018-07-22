#include "TransformComponent.h"



game::component::TransformComponent::TransformComponent(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& table) : Component(entity)
{
	parse_table(table);
}

game::component::TransformComponent::~TransformComponent()
{
}

game::component::Component* game::component::TransformComponent::copy() const
{
	return nullptr;
}

void game::component::TransformComponent::parse_table(const LuaIntf::LuaRef& table)
{
}
