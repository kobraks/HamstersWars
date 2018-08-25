#pragma once
#include <SFML/System/NonCopyable.hpp>

#include <vector>
#include <memory>
#include <string>

#include "Entity.h"
#include "Log.h"

namespace LuaIntf {
	class LuaRef;
}

namespace game
{
	class EntityLoader : sf::NonCopyable
	{
	public:
		static std::vector<std::shared_ptr<Entity>> load(const std::string& file);
		static std::shared_ptr<Entity> load_entity(const std::string& name, const std::string& file);

	private:
		static void load_components_from_file(std::shared_ptr<Entity> entity, const std::string& entity_name, const std::string& file);

		static std::shared_ptr<Entity> load_entity(const std::string& type, const LuaIntf::LuaRef& table);
		static std::vector<std::shared_ptr<Entity>> get_entities(const LuaIntf::LuaRef& table);
		static void get_component(std::shared_ptr<Entity> entity, const std::string& component, const LuaIntf::LuaRef& table);

		template<class T>
		static void add_component(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& table, const std::string& component_name = "COMPONENT_NAME");

		EntityLoader() = default;
	};

	template <class T>
	void EntityLoader::add_component(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& table, const std::string& component_name)
	{
		LOG(LOG_DEBUG, "Loading %s component", component_name.c_str());

		entity->add_component<T>(new T(entity, table));
	}
}
