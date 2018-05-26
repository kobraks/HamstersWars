#include "Component.h"

game::component::Component::Component(Entity* owner) : owner_(owner)
{
}

game::Entity* game::component::Component::get_owner() const
{
	return owner_;
}

void game::component::Component::set_owner(Entity* entity)
{
	owner_ = entity;
}
