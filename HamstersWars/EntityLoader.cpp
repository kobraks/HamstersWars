#include "EntityLoader.h"

#include <algorithm>

#include "Log.h"
#include "Defines.h"
#include "Script.h"
#include "Defines.h"
#include "Utils.h"

#pragma region components
#include "ScriptHandler.h"
#include "ColliderComponent.h"
#include "GraphicComponent.h"
#include "TransformComponent.h"
#pragma endregion

#define TO_STRING(x) #x

/**
 * \brief loads entities from slected file
 * \param file path to entities file
 * \return table_type of loaded entitiess
 */
std::vector<std::shared_ptr<game::Entity>> game::EntityLoader::load(const std::string& file)
{
	try
	{
		LOG(LOG_INFO, "Loading entitys from %s", file.c_str());
		lua::Script::do_file(file);
	}
	catch(LuaIntf::LuaException& lua)
	{
		LOG(LOG_ERROR, "Lua throws exception: %s", lua.what());
		return std::vector<std::shared_ptr<game::Entity>>();
	}

	return get_entities(LuaIntf::LuaRef(lua::Script::lua(), "entities"));
}

std::shared_ptr<game::Entity> game::EntityLoader::load_entity(const std::string & name, const std::string & file)
{
	std::shared_ptr<Entity> entity(new Entity());
	entity->set_type(name);
	load_components_from_file(entity, name, file);

	return entity;
}

/**
* \brief loads components of entity and populate that entity form file
* \param entity pointer to entity that should be populated
* \param entity_name entity name
* \param file path to file of entity components
*/
void game::EntityLoader::load_components_from_file(std::shared_ptr<Entity> entity, const std::string& entity_name,
	const std::string& file)
{
	try
	{
		LOG(LOG_DEBUG, "Opening file: %s", std::string(LUA_SCRIPTS_PATH + file).c_str());

		lua::Script::do_file(std::string(LUA_SCRIPTS_PATH + file).c_str());
	}
	catch (LuaIntf::LuaException& lua)
	{
		LOG(LOG_ERROR, "Lua throws exception: %s", lua.what());
		return;
	}

	LuaIntf::LuaRef components;

	try
	{
		components = LuaIntf::LuaRef(lua::Script::lua(), entity_name.c_str());

		if (!components.isValid() || !components.isTable())
		{
			LOG(LOG_WARNING, "Entity %s has no components", entity->get_type());
			return;
		}
	}
	catch (LuaIntf::LuaException& ex)
	{
		LOG(LOG_ERROR, "Lua throws exception: %s", ex.what());
		return;
	}

	LOG(LOG_DEBUG, "Populate %s with components", entity->get_type());
	for (auto component : components)
		get_component(entity, component.key<std::string>(), component.value());
}

/**
 * \brief loads entities form table_type
 * \param table table_type of entities
 * \return table_type of entities with its components
 */
std::vector<std::shared_ptr<game::Entity>> game::EntityLoader::get_entities(const LuaIntf::LuaRef& table)
{
	std::vector<std::shared_ptr<Entity>> result;
	if (!table.isValid() || !table.isTable())
		return result;

	for (auto& entity_table : table)
	{
		try
		{
			auto entity = load_entity(entity_table.key<std::string>(), entity_table.value());

			if (entity)
				result.push_back(entity);
			else
				LOG(LOG_ERROR, "Unable to load entity %s", entity_table.key<std::string>().c_str());
		}
		catch (LuaIntf::LuaException& ex)
		{
			LOG(LOG_ERROR, "Lua throws exception: %s", ex.what());

			try
			{
				LOG(LOG_ERROR, "Unable to load entity %s", entity_table.key<std::string>().c_str());
			}
			catch(LuaIntf::LuaRef&)
			{ }
		}
	}
	return result;
}

/**
 * \brief loads and populate the entity
 * \param type name of entity
 * \param table entity table_type if is empty then is treaten as path to entity file same if has path variable
 * \return returns entity with its components or nullptr when was an error of entity is empty
 */
std::shared_ptr<game::Entity> game::EntityLoader::load_entity(const std::string& type, const LuaIntf::LuaRef& table)
{
	std::shared_ptr<Entity> entity(new Entity);
	entity->set_type(type);

	try
	{
		LOG(LOG_DEBUG, "Loading entity %s", type.c_str());

		//if table is and path or contains a path element.
		if (utils::is_path(table))
		{
			LOG(LOG_DEBUG, "Entity detected as file");
			load_components_from_file(entity, type, utils::get_path(table));
		}
		//entity is in entities file and is table_type then load the components
		else if (table.isTable())
		{
			LOG(LOG_DEBUG, "Entity is an table");
			for (auto component : table)
				get_component(entity, component.key<std::string>(), component.value());
		}
		else
			return nullptr;
	}
	catch(LuaIntf::LuaException& ex)
	{
		LOG(LOG_ERROR, "Lua throws exception: %s", ex.what());
		return nullptr;
	}
	
	return entity;
}

/**
 * \brief loads a single component to entity
 * \param entity pointer to entity that should be populated
 * \param component name of component
 * \param ref table_type of definitions of component
 */
void game::EntityLoader::get_component(std::shared_ptr<Entity> entity, const std::string& component,
	const LuaIntf::LuaRef& ref)
{
	if (!ref.isValid() || !ref.isTable() || !entity)
		return;

	auto component_name = component;
	std::transform(component_name.begin(), component_name.end(), component_name.begin(), ::toupper);

	if (component_name == SCRIPT_COMPONENT)
		add_component<component::ScriptHandler>(entity, ref, component);
	else if (component_name == COLLIDER_COMPONENT)
		add_component<component::ColliderComponent>(entity, ref, component);
	else if (component_name == GRAPHIC_COMPONENT)
		add_component<component::GraphicComponent>(entity, ref, component);
	else if (component_name == TRANSFORM_COMPONENT)
		add_component<component::TransformComponent>(entity, ref, component);
	else
	{
		LOG(LOG_WARNING, "Unrecognized %s component %s", entity->get_type().c_str(), component.c_str());
	}
}
