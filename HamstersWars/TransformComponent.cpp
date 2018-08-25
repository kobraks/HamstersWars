#include "TransformComponent.h"

#include "Utils.h"
#include "Log.h"
#include "GameException.h"

game::component::TransformComponent::TransformComponent(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& table) : Component(entity)
{
	parse_table(table);
}

game::component::TransformComponent::~TransformComponent()
{
}

game::component::Component* game::component::TransformComponent::copy() const
{
	return new TransformComponent(*this);
}

std::string game::component::TransformComponent::get_name() const
{
	return "TransformComponent";
}

void game::component::TransformComponent::parse_table(const LuaIntf::LuaRef& table)
{
	
	for (auto element : table)
	{
		if (utils::to_upper_copy(element.key<std::string>()) == "POSITION")
		{
			LOG(LOG_DEBUG, "Reading position");
			try
			{
				set_position(utils::get_vector3d(element.value()));
				LOG(LOG_DEBUG1, "Position: x = %f, y = %f, z = %f", get_position().x, get_position().y, get_position().z);
			}
			catch(exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown %s", ex.what());
			}

		}
		else if (utils::to_upper_copy(element.key<std::string>()) == "SCALE")
		{
			LOG(LOG_DEBUG, "Reading scale");
			try
			{
				set_scale(utils::get_vector3d(element.value()));
				LOG(LOG_DEBUG1, "Scale: x = %f, y = %f, z = %f", get_scale().x, get_scale().y, get_scale().z);
			}
			catch(exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown %s", ex.what());
			}
		}
		else if (utils::to_upper_copy(element.key<std::string>()) == "ROTATE")
		{
			LOG(LOG_DEBUG, "Reading rotation");
			try
			{
				set_rotation(utils::get_vector3d(element.value()));
				LOG(LOG_DEBUG1, "Rotate: x = %f, y = %f, z = %f", get_rotation().x, get_rotation().y, get_rotation().z);
			}
			catch (exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown %s", ex.what());
			}
		}
		else
		{
			LOG(LOG_WARNING, "Unable to recognize gived table_type key: %s", element.key<std::string>().c_str());
		}
	}
}
