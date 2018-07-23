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

void game::component::TransformComponent::parse_table(const LuaIntf::LuaRef& table)
{
	
	for (auto element : table)
	{
		if (utils::to_upper_copy(element.key<std::string>()) == "POSITION")
		{
			Log::level() = Log::log_info;
			Log::print("Reading position");
			try
			{
				set_position(utils::get_vector3d(element.value()));
				Log::print("Position: x = %f, y = %f, z = %f", get_position().x, get_position().y, get_position().z);
			}
			catch(exception::GameException& ex)
			{
				Log::level() = Log::log_error;
				Log::print(ex.what());
			}

		}
		else if (utils::to_upper_copy(element.key<std::string>()) == "SCALE")
		{
			Log::level() = Log::log_info;
			Log::print("Reading scale");
			try
			{
				set_scale(utils::get_vector3d(element.value()));
				Log::print("Scale: x = %f, y = %f, z = %f", get_scale().x, get_scale().y, get_scale().z);
			}
			catch(exception::GameException& ex)
			{
				Log::level() = Log::log_error;
				Log::print(ex.what());
			}
		}
		else if (utils::to_upper_copy(element.key<std::string>()) == "ROTATE")
		{
			Log::level() = Log::log_info;
			Log::print("Reading rotation");
			try
			{
				set_rotation(utils::get_vector3d(element.value()));
				Log::print("Rotate: x = %f, y = %f, z = %f", get_rotation().x, get_rotation().y, get_rotation().z);
			}
			catch (exception::GameException& ex)
			{
				Log::level() = Log::log_error;
				Log::print(ex.what());
			}
		}
		else
		{
			Log::level() = Log::log_warning;
			Log::print("Unable to recognize gived table key: %s", element.key<std::string>().c_str());
		}
	}
}
