#include "EntityLoader.h"

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include "Log.h"
#include "Defines.h"

#pragma region components
#include "ScriptHandler.h"
#include "ColliderComponent.h"
#include "GraphicComponent.h"
#pragma endregion

#define TO_STRING(x) #x

/**
 * \brief loads entities from slected file
 * \param file path to entities file
 * \return table of loaded entitiess
 */
std::vector<std::shared_ptr<game::Entity>> game::EntityLoader::load(const std::string& file)
{
	std::shared_ptr<LuaIntf::LuaContext> L(new LuaIntf::LuaContext());

	try
	{
		L->doFile(file.c_str());
	}
	catch(LuaIntf::LuaException& lua)
	{
		Log::write_error("lua throws exception: ", lua.what());
		return std::vector<std::shared_ptr<game::Entity>>();
	}
	return get_entities(LuaIntf::LuaRef(*L, "entities"), L);
}

/**
 * \brief loads entities form table
 * \param table table of entities
 * \param context lua context
 * \return table of entities with its components
 */
std::vector<std::shared_ptr<game::Entity>> game::EntityLoader::get_entities(
	const LuaIntf::LuaRef& table, const std::shared_ptr<LuaIntf::LuaContext>& context)
{
	std::vector<std::shared_ptr<Entity>> result;
	if (!table.isValid() || !table.isTable())
		return result;

	for (auto& entity_table : table)
	{
		try
		{
			auto entity = load_entity(entity_table.key<std::string>(), entity_table.value(), context);

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
 * \param context lua context
 * \return returns entity with its components or nullptr when was an error of entity is empty
 */
std::shared_ptr<game::Entity> game::EntityLoader::load_entity(const std::string& type, const LuaIntf::LuaRef& table,
                                                              const std::shared_ptr<LuaIntf::LuaContext>& context)
{
	std::shared_ptr<Entity> entity(new Entity);

	try
	{

		//if table is table and has path then it should be loaded from path file
		if (table.isTable() && table.has("path"))
			load_components_from_file(entity, type, table["path"].value<std::string>());

		//if table is not table and is value then treat that value as path to entity file
		else if (!table.isTable() && !table.toValue<std::string>().empty())
			load_components_from_file(entity, type, table.toValue<std::string>());
		//entity is in entities file and is table then load the components
		else if (table.isTable())
			for (auto component : table)
				get_component(entity, component.key<std::string>(), component.value(), context);
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
 * \brief loads components of entity and populate that entity form file
 * \param entity pointer to entity that should be populated
 * \param entity_name entity name
 * \param file path to file of entity components
 */
void game::EntityLoader::load_components_from_file(std::shared_ptr<Entity> entity, const std::string& entity_name,
                                                   const std::string& file)
{
	std::shared_ptr<LuaIntf::LuaContext> L(new LuaIntf::LuaContext);
	
	try
	{
		L->doFile(std::string(LUA_SCRIPTS_PATH + file).c_str());
	}
	catch (LuaIntf::LuaException& lua)
	{
		Log::write_error("lua throws exception: ", lua.what());

		return;
	}

	LuaIntf::LuaRef components;

	try
	{
		components = LuaIntf::LuaRef(*L, entity_name.c_str());

		if (!components.isValid() || !components.isTable())
			return;


	}
	catch (LuaIntf::LuaException& ex)
	{
		Log::write_error("unable to read entity table lua throws exception: ", ex.what());
		return;
	}

	for (auto component : components)
		get_component(entity, component.key<std::string>(), component.value(), L);
}

/**
 * \brief loads a single component to entity
 * \param entity pointer to entity that should be populated
 * \param component name of component
 * \param ref table of definitions of component
 * \param context lua context
 */
void game::EntityLoader::get_component(std::shared_ptr<Entity> entity, const std::string& component,
	const LuaIntf::LuaRef& ref, const std::shared_ptr<LuaIntf::LuaContext>& context)
{
	if (!ref.isValid() || !ref.isTable() || !entity)
		return;

	if (component == "Script")
	{
		auto component = new component::ScriptHandler(entity, context, ref["update"]);
		entity->add_component<component::ScriptHandler>(component);
	}
	else if (component == "Collider")
	{
		auto component = new component::ColliderComponent(entity);
		entity->add_component<component::ColliderComponent>(component);
	}
	else if (component == "Graphic")
	{
		auto component = new component::GraphicComponent(entity, ref);
		entity->add_component<component::GraphicComponent>(component);
	}
	else
	{
		Log::level() = Log::log_warning;
		Log::print("Unrecognize component: %s", component.c_str());
	}
}
