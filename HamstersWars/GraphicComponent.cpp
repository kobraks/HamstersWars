#include "GraphicComponent.h"
#include "Entity.h"



game::component::GraphicComponent::GraphicComponent(std::shared_ptr<Entity> owner, const LuaIntf::LuaRef& table) : Component(owner)
{
}

game::component::GraphicComponent::~GraphicComponent()
{
}

bool game::component::GraphicComponent::drawable() const
{
	return drawable_;
}

void game::component::GraphicComponent::draw() const
{
}

game::component::Component* game::component::GraphicComponent::copy() const
{
	return nullptr;
}

void game::component::GraphicComponent::draw_forced_texture()
{

}
