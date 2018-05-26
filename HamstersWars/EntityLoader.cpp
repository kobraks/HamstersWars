#include "EntityLoader.h"

#include <Lua/lua.hpp>
#include <Lua/LuaIntf.h>
#include "Log.h"

std::vector<std::shared_ptr<game::Entity>> game::EntityLoader::load(const std::string& file)
{
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);

	LuaIntf::LuaContext L;

	try
	{
		L.doFile(file.c_str());
	}
	catch(LuaIntf::LuaException& lua)
	{
		Log::write_error("lua throws exception: ", lua.what());
		return std::vector<std::shared_ptr<game::Entity>>();
	}
	return get_entities(LuaIntf::LuaRef(L, "entities"));
}

std::vector<std::shared_ptr<game::Entity>> game::EntityLoader::get_entities(const LuaIntf::LuaRef& ref)
{
	std::vector<std::shared_ptr<Entity>> result;
	if (!ref.isValid() || !ref.isTable())
		return result;

	for (auto& entity_table : ref)
	{
		try
		{
			auto entity = load_entity(entity_table.key<std::string>(), entity_table.value());

			if (entity)
				result.push_back(entity);
			else
			{
				Log::level() = Log::log_error;
				Log::write("Unable to load entity %s", entity_table.key<std::string>());
			}
		}
		catch (LuaIntf::LuaException& ex)
		{
			Log::write_error("lua throws exception: ", ex.what());
			

			try
			{
				Log::level() = Log::log_error;
				Log::write("Unable to load entity %s", entity_table.key<std::string>());
			}
			catch(LuaIntf::LuaRef&)
			{ }
		}
	}
	return result;
}

std::shared_ptr<game::Entity> game::EntityLoader::load_entity(const std::string& type, const LuaIntf::LuaRef& ref)
{
	std::shared_ptr<Entity> entity(new Entity);

	try
	{
		if (ref.isTable() && ref.has("path"))
			load_components_from_file(entity, type, ref["path"].value<std::string>());
		else if (!ref.isTable() && !ref.toValue<std::string>().empty())
			load_components_from_file(entity, type, ref.toValue<std::string>());
		else if (ref.isTable())
			for (auto component : ref)
				get_component(entity, component.key<std::string>(), component.value());
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

void game::EntityLoader::load_components_from_file(std::shared_ptr<Entity> entity, const std::string& entity_name, const std::string& file)
{
	LuaIntf::LuaContext L;
	
	try
	{
		L.doFile(file.c_str());
	}
	catch (LuaIntf::LuaException& lua)
	{
		Log::write_error("lua throws exception: ", lua.what());

		return;
	}

	LuaIntf::LuaRef components;

	try
	{
		components = LuaIntf::LuaRef(L, entity_name.c_str());

		if (!components.isValid() || !components.isTable())
			return;


	}
	catch (LuaIntf::LuaException& ex)
	{
		Log::write_error("unable to read entity table lua throws exception: ", ex.what());
		return;
	}

	for (auto component : components)
		get_component(entity, component.key<std::string>(), component.value());
}

void game::EntityLoader::get_component(std::shared_ptr<Entity> entity, const std::string& component,
	const LuaIntf::LuaRef& ref)
{
	if (!ref.isValid() || !ref.isTable() || !entity)
		return;

	//TODO
}
