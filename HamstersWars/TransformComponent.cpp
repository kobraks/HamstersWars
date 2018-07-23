#include "TransformComponent.h"

#include "LuaUtils.h"
#include "UnknownTableElementException.h"
#include "Utils.h"

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
	
	for (auto element : table)
	{
		if (utils::to_upper_copy(element.key<std::string>()) == "POSITION")
			set_position(utils::get_vector3d(element.value()));
		else if (utils::to_upper_copy(element.key<std::string>()) == "SCALE")
			set_scale(utils::get_vector3d(element.value()));
		else if (utils::to_upper_copy(element.key<std::string>()) == "ROTATE")
			set_position(utils::get_vector3d(element.value()));
		else
			throw exception::UnknownTableElementException();
	}
}
