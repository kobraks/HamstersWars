#include "stdafx.h"
#include "EntityLoader.h"
#include "Entity.h"
#include "Script.h"

#include "GraphicComponent.h"
#include "TransformComponent.h"
#include "ScriptComponent.h"

namespace game
{
	namespace entity
	{
		Entity* EntityLoader::load_entity(const std::string& entity_name, const std::string& path)
		{
			Entity* entity = nullptr;

			try
			{
				lua::Script::do_file(path);
				entity = new Entity(entity_name);
				parse_table(LuaIntf::LuaRef(lua::Script::lua(), entity_name.c_str()), entity);
			}
			catch(std::exception& ex)
			{
				LOG(LOG_ERROR, "Unable to load entity from %s, %s", path.c_str(), ex.what());
			}

			return entity;
		}

		void EntityLoader::parse_table(LuaIntf::LuaRef table, Entity* entity)
		{
			if (!entity)
				return;

			for (auto row : table)
			{
				auto key = utils::to_upper_copy(utils::trim_copy(row.key<std::string>()));

				if (key == GRAPHIC_COMPONENT)
					add_component<component::GraphicComponent>(row.key<std::string>(), row.value(), entity);
				else if (key == TRANSFORM_COMPONENT)
					add_component<component::TransformComponent>(row.key<std::string>(), row.value(), entity);
				else if (key == SCRIPT_COMPONENT)
				{
					add_component<component::ScriptComponent>(row.key<std::string>(), row.value(), entity);
					entity->components.get<component::ScriptComponent>()->set_entity(entity);
				}
				else
					LOG(LOG_WARNING, "Unrecognized component %s", row.key<std::string>().c_str());
			}
		}
	}
}
