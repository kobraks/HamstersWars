#include "stdafx.h"
#include "Component.h"

game::component::Component::Component(std::shared_ptr<Entity> owner) : owner_(owner)
{
}

std::shared_ptr<game::Entity> game::component::Component::get_owner() const
{
	return owner_;
}

void game::component::Component::set_owner(std::shared_ptr<Entity> entity)
{
	owner_ = entity;
}
