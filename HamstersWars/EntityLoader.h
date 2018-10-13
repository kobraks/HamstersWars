#pragma once
#include "Entity.h"

namespace game
{
	namespace entity
	{
		class Entity;

		class EntityLoader
		{
		public:
			static Entity* load_entity(const std::string& entity_name, const std::string& path);
			
		private:
			static void parse_table(LuaIntf::LuaRef table, Entity* entity);

			template<class ComponentType>
			static void add_component(const std::string& component_name, LuaIntf::LuaRef component_table, Entity* entity);
		};

		template <class ComponentType>
		void EntityLoader::add_component(const std::string& component_name, LuaIntf::LuaRef component_table, Entity* entity)
		{
			LOG(LOG_DEBUG, "Loading %s component", component_name.c_str());

			try
			{
				auto component = new ComponentType();
				component->parse_table(component_table);
				entity->components.add(typeid(ComponentType), component);
			}
			catch(LuaIntf::LuaException& ex)
			{
				LOG(LOG_ERROR, "Unable to load component: %s", ex.what());
			}
		}
	}
}

