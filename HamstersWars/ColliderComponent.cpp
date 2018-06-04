#include "ColliderComponent.h"



game::component::ColliderComponent::~ColliderComponent()
{
}

game::component::ColliderComponent::ColliderComponent(std::shared_ptr<Entity> owner) : Component(owner)
{
}

game::component::Component* game::component::ColliderComponent::copy() const
{
	return nullptr;
	//return new ColliderComponent(*this);
}
