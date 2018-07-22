#include "ColliderComponent.h"



game::component::ColliderComponent::~ColliderComponent()
{
}

void game::component::ColliderComponent::parse_table(const LuaIntf::LuaRef& table)
{
}

game::component::ColliderComponent::ColliderComponent(std::shared_ptr<Entity> owner, const LuaIntf::LuaRef& table) : Component(owner)
{
	parse_table(table);
}

game::component::Component* game::component::ColliderComponent::copy() const
{
	return nullptr;
	//return new ColliderComponent(*this);
}
