#include "stdafx.h"
#include "TransformComponent.h"


namespace game::component
{
	TransformComponent::TransformComponent() : Component(TRANSFORM_COMPONENT)
	{
		properties.add<gl::Vector3D>("position", gl::Vector3D());
		properties.add<gl::Vector3D>("rotation", gl::Vector3D());
		properties.add<gl::Vector3D>("scale", gl::Vector3D());
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::parse_table(LuaIntf::LuaRef table)
	{
		if (!table.isTable())
		{
			LOG(LOG_ERROR, "Unable to parse content");
			return;
		}

		for (auto row : table)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(row.key<std::string>()));

			if (key == TRANSFORM_POSITION)
			{
				LOG(LOG_DEBUG, "Reading position");
				try
				{
					set_position(utils::get_vector3d(row.value()));
					LOG(LOG_DEBUG1, "Position: x = %f, y = %f, z = %f", get_position().x, get_position().y, get_position().z);
				}
				catch (exception::GameException& ex)
				{
					LOG(LOG_ERROR, "Exception was thrown %s", ex.what());
				}
			}
			else if (key == TRANSFORM_SCALE)
			{
				LOG(LOG_DEBUG, "Reading scale");
				try
				{
					set_scale(utils::get_vector3d(row.value()));
					LOG(LOG_DEBUG1, "Scale: x = %f, y = %f, z = %f", get_scale().x, get_scale().y, get_scale().z);
				}
				catch (exception::GameException& ex)
				{
					LOG(LOG_ERROR, "Exception was thrown %s", ex.what());
				}
			}
			else if (key == TRANSFORM_ROTATE)
			{
				LOG(LOG_DEBUG, "Reading rotation");
				try
				{
					set_rotation(utils::get_vector3d(row.value()));
					LOG(LOG_DEBUG1, "Rotate: x = %f, y = %f, z = %f", get_rotation().x, get_rotation().y, get_rotation().z);
				}
				catch (exception::GameException& ex)
				{
					LOG(LOG_ERROR, "Exception was thrown %s", ex.what());
				}
			}
			else
			{
				LOG(LOG_WARNING, "Unable to recognize gived table_type key: %s", row.key<std::string>().c_str());
			}
		}
	}

	interfaces::Component* TransformComponent::clone() const
	{
		auto component = new TransformComponent();
		component->properties = properties;

		return component;
	}

	void TransformComponent::set_position(const gl::Vector3D& position)
	{
		properties.set<gl::Vector3D>("position", position);
	}

	gl::Vector3D TransformComponent::get_position() const
	{
		return properties.get<gl::Vector3D>("position");
	}

	void TransformComponent::set_scale(const gl::Vector3D& scale)
	{
		properties.set<gl::Vector3D>("scale", scale);
	}

	gl::Vector3D TransformComponent::get_scale() const
	{
		return properties.get<gl::Vector3D>("scale");
	}

	void TransformComponent::set_rotation(const gl::Vector3D& rotation)
	{
		properties.set<gl::Vector3D>("rotation", rotation);
	}

	gl::Vector3D TransformComponent::get_rotation() const
	{
		return properties.get<gl::Vector3D>("rotation");
	}
}
