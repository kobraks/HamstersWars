#include "EntityLoader.h"

#include <algorithm>

#include "Log.h"
#include "Defines.h"
#include "Script.h"
#include "Defines.h"

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
 * \return table of loaded entitiess
 */
std::vector<std::shared_ptr<game::Entity>> game::EntityLoader::load(const std::string& file)
{
	try
	{
		Log::level() = Log::log_info;
		Log::print("Loading file %s", file.c_str());
		lua::Script::do_file(file);
	}
	catch(LuaIntf::LuaException& lua)
	{
		Log::write_error("lua throws exception: ", lua.what());
		return std::vector<std::shared_ptr<game::Entity>>();
	}

	return get_entities(LuaIntf::LuaRef(lua::Script::lua(), "entities"));
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
		Log::level() = Log::log_info;
		Log::print("Opening file: %s", std::string(LUA_SCRIPTS_PATH + file).c_str());

		lua::Script::do_file(std::string(LUA_SCRIPTS_PATH + file).c_str());
	}
	catch (LuaIntf::LuaException& lua)
	{
		Log::write_error("lua throws exception: ", lua.what());

		return;
	}

	LuaIntf::LuaRef components;

	try
	{
		components = LuaIntf::LuaRef(lua::Script::lua(), entity_name.c_str());

		if (!components.isValid() || !components.isTable())
		{
			Log::level() = Log::log_warning;
			Log::print("insite doeas not exist any of component table");
			return;
		}
	}
	catch (LuaIntf::LuaException& ex)
	{
		Log::write_error("unable to read entity table lua throws exception: ", ex.what());
		return;
	}

	Log::level() = Log::log_info;
	Log::print("Loading components");
	for (auto component : components)
		get_component(entity, component.key<std::string>(), component.value());
}

/**
 * \brief loads entities form table
 * \param table table of entities
 * \return table of entities with its components
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
			{
				Log::level() = Log::log_error;
				Log::print("Unable to load entity %s", entity_table.key<std::string>());
			}
		}
		catch (LuaIntf::LuaException& ex)
		{
			Log::write_error("lua throws exception: ", ex.what());
			

			try
			{
				Log::level() = Log::log_error;
				Log::print("Unable to load entity %s", entity_table.key<std::string>());
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
 * \param table entity table if is empty then is treaten as path to entity file same if has path variable
 * \return returns entity with its components or nullptr when was an error of entity is empty
 */
std::shared_ptr<game::Entity> game::EntityLoader::load_entity(const std::string& type, const LuaIntf::LuaRef& table)
{
	std::shared_ptr<Entity> entity(new Entity);
	entity->set_type(type);

	try
	{
		Log::level() = Log::log_info;
		Log::print("Loading entity: %s", type.c_str());
		//if table is table and has path then it should be loaded from path file
		if (table.isTable() && table.has("path"))
		{
			Log::level() = Log::log_info;
			Log::print("entity has a file path so its will be loaded from it");
			load_components_from_file(entity, type, table["path"].value<std::string>());
		}

		//if table is not table and is value then treat that value as path to entity file
		else if (!table.isTable() && !table.toValue<std::string>().empty())
		{
			Log::level() = Log::log_info;
			Log::print("entity is a file path");
			load_components_from_file(entity, type, table.toValue<std::string>());
		}
		//entity is in entities file and is table then load the components
		else if (table.isTable())
		{
			Log::level() = Log::log_info;
			Log::print("entity is a table");
			for (auto component : table)
				get_component(entity, component.key<std::string>(), component.value());
		}
		else
			return nullptr;
	}
	catch(LuaIntf::LuaException& ex)
	{
		Log::write_error("lua error", ex.what());

		return nullptr;
	}
	
	return entity;
}

/**
 * \brief loads a single component to entity
 * \param entity pointer to entity that should be populated
 * \param component name of component
 * \param ref table of definitions of component
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
		Log::level() = Log::log_warning;
		Log::print("Unrecognize component: %s", component.c_str());
	}
}
