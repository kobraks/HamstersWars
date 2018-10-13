#include "stdafx.h"
#include "EntityScriptHandler.h"
#include "Log.h"

#define ADD_FUNCTION(x) addFunction(#x, &EntityScriptHandler::x)
#define ADD_FUNCTION_PARAMS(x,params) addFunction(#x, &EntityScriptHandler::x, params)

namespace game::entity
{
	EntityScriptHandler::EntityScriptHandler()
	{
	}

	EntityScriptHandler::EntityScriptHandler(Entity* entity)
	{
	}

	void EntityScriptHandler::add_property(const property_id_type& property_id, LuaIntf::LuaRef lua)
	{
		entity_->properties.add<LuaIntf::LuaRef>(property_id, lua);
	}

	LuaIntf::LuaRef EntityScriptHandler::get_property(const property_id_type& property_id)
	{
		try
		{
			return entity_->properties.get<LuaIntf::LuaRef>(property_id);

		}
		catch(std::exception& ex)
		{
			LOG(LOG_WARNING, "No property with id %s", property_id.c_str());
			return LuaIntf::LuaRef();
		}
	}

	void EntityScriptHandler::clone()
	{
	}

	void EntityScriptHandler::destroy()
	{
	}

	void EntityScriptHandler::register_class(LuaIntf::LuaBinding& binding) const
	{
		LOG(LOG_INFO, "Registering entity functions");

		binding
			.beginClass<EntityScriptHandler>("EntityScriptHandler")
				.ADD_FUNCTION(add_property)
				.ADD_FUNCTION(get_property)
				.ADD_FUNCTION(clone)
				.ADD_FUNCTION(destroy)
			.endClass();
	}
}

/*void game::script::EntityScriptHandler::register_class(LuaIntf::LuaBinding& binding) const
{
	LOG(LOG_INFO, "Registering lua handlers");
	using namespace game::script;

	binding
		.beginClass<EntityScriptHandler>("EntityScriptHandler")
		.ADD_FUNCTION(get_elapsed_time)
		.ADD_FUNCTION_PARAMS(scale, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(translate, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(rotate, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(set_position, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(move, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION(get_position)
		.ADD_FUNCTION(is_colliding)
		.ADD_FUNCTION(destroy)
		.endClass();
}*/
