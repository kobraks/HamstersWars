#include "Entity.h"


game::Entity::Entity()
{
}

game::Entity::Entity(const Entity& entity)
{
	type_ = entity.type_;

	components_ = entity.components_;

	for (auto& component : components_)
		component.second = component.second->copy();
}

game::Entity::Entity(Entity&& entity) noexcept
{
	type_ = std::move(entity.type_);

	components_ = std::move(entity.components_);
}

game::Entity::~Entity()
{
	for (auto& component : components_)
		delete component.second;
}

game::Entity* game::Entity::copy() const
{
	return new Entity(*this);
}

std::string game::Entity::get_type() const
{
	return type_;
}

void game::Entity::set_type(const std::string& type)
{
	type_ = type;
}

void game::Entity::add_component(std::type_index type_index, component::Component* component)
{
	auto comp = components_.find(type_index);

	if (comp != components_.end())
		delete comp->second;

	components_[type_index] = component;
}
