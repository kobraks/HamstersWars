#include "ScriptHandler.h"
#include "EntityScriptHandler.h"
#include "Script.h"
#include "Log.h"
#include "defines.h"
#include "utils.h"
#include "NotFunctionException.h"

#define ADD_FUNCTION(x) addFunction(#x, &EntityScriptHandler::x)
#define ADD_FUNCTION_PARAMS(x,params) addFunction(#x, &EntityScriptHandler::x, params)

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner) : ScriptHandler()
{
	set_owner(owner);
}

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update) : ScriptHandler(owner)
{
	set_on_update(update);
}

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update,
	const std::string& create) : ScriptHandler(owner, update)
{
	set_on_create(create);
}

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update,
	const std::string& create, const std::string& destroy) : ScriptHandler(owner, update, create)
{
	set_on_destroy(destroy);
}

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner, const std::string& update,
	const std::string& create, const std::string& destroy, const std::string& copy) : ScriptHandler(owner, update, create, destroy)
{
	set_on_copy(copy);
}

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& component_table) : ScriptHandler(entity)
{
	parse_table(component_table);

}

void game::component::ScriptHandler::set_on_update(const std::string& code)
{
	//TODO
}

void game::component::ScriptHandler::set_on_update(const LuaIntf::LuaRef& function)
{
	LOG(LOG_DEBUG, "Adding on_update function to %s", get_owner()->get_type().c_str());
	if (!is_function(function))
		throw exception::NotFunctionExcpetion(UPDATE_FUNCTION);

	update_ = function;
}

void game::component::ScriptHandler::set_on_destroy(const std::string& code)
{
	//TODO
}

void game::component::ScriptHandler::set_on_destroy(const LuaIntf::LuaRef& function)
{
	LOG(LOG_DEBUG, "Adding on_destroy function to %s", get_owner()->get_type().c_str());
	if (!is_function(function))
		throw exception::NotFunctionExcpetion(DESTROY_FUNCTION);

	destroy_ = function;
}

void game::component::ScriptHandler::set_on_copy(const std::string& code)
{
	//TODO
}

void game::component::ScriptHandler::set_on_copy(const LuaIntf::LuaRef& function)
{
	LOG(LOG_DEBUG, "Adding on_copy function to %s", get_owner()->get_type().c_str());
	if (!is_function(function))
		throw exception::NotFunctionExcpetion(COPY_FUNCTION);

	copy_ = function;
}

void game::component::ScriptHandler::set_on_create(const std::string& code)
{
	//TODO
}


void game::component::ScriptHandler::set_on_create(const LuaIntf::LuaRef& function)
{
	LOG(LOG_DEBUG, "Adding on_create function to %s", get_owner()->get_type().c_str());
	if (!is_function(function))
		throw exception::NotFunctionExcpetion(CREATE_FUNCTION);

	create_ = function;
}

void game::component::ScriptHandler::set_on_collision(const std::string& code)
{
	//TODO
}

void game::component::ScriptHandler::set_on_collision(const LuaIntf::LuaRef& function)
{
	LOG(LOG_DEBUG, "Adding on_collision function to %s", get_owner()->get_type().c_str());
	if (!is_function(function))
		throw exception::NotFunctionExcpetion(COLLISION_FUNCTION);

	create_ = function;
}


game::component::Component* game::component::ScriptHandler::copy() const
{
	return new ScriptHandler(*this);
}

std::string game::component::ScriptHandler::get_name() const
{
	return "ScriptHandler";
}

void game::component::ScriptHandler::on_update()
{
	run_function(update_);
}

void game::component::ScriptHandler::on_destroy()
{
	run_function(destroy_);
}

void game::component::ScriptHandler::on_create()
{
	run_function(create_);
}

void game::component::ScriptHandler::on_copy()
{
	run_function(copy_);
}

void game::component::ScriptHandler::register_class(LuaIntf::LuaBinding& binding) const
{
	using namespace script;

	binding
		.beginClass<glm::vec3>("vec3")
			.addConstructor(LUA_ARGS(float, float, float))
			.addVariable("x", &glm::vec3::x, true)
			.addVariable("y", &glm::vec3::y, true)
			.addVariable("z", &glm::vec3::z, true)
		.endClass()
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
}

game::component::ScriptHandler::ScriptHandler() : Component(nullptr)
{
	lua::Script::register_class<ScriptHandler>(this);

}

void game::component::ScriptHandler::parse_table(const LuaIntf::LuaRef& table)
{
	assert(table.isTable());

	for (auto element : table)
	{
		auto bigger_key = utils::to_upper_copy(element.key<std::string>());
		auto value = element.value();

		if (bigger_key == UPDATE_FUNCTION)
		{
			try
			{
				set_on_update(value);
			}
			catch (exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown: %s", ex.what());
			}
		}
		else if (bigger_key == COPY_FUNCTION)
		{
			try
			{
				set_on_copy(value);
			}
			catch (exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown: %s", ex.what());
			}
		}
		else if (bigger_key == CREATE_FUNCTION)
		{
			try
			{
				set_on_create(value);
			}
			catch (exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown: %s", ex.what());
			}
		}
		else if (bigger_key == DESTROY_FUNCTION)
		{
			try
			{
				set_on_destroy(value);
			}
			catch (exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown: %s", ex.what());
			}
		}
		else if (bigger_key == COLLISION_FUNCTION)
		{
			try
			{
				set_on_collision(value);
			}
			catch (exception::GameException& ex)
			{
				LOG(LOG_ERROR, "Exception was thrown: %s", ex.what());
			}
		}
		else
			LOG(LOG_WARNING, "Unable to recognize gived table_type key: %s", element.key<std::string>().c_str());
		}

}

void game::component::ScriptHandler::run_function(LuaIntf::LuaRef& function) const
{
	if (function)
	{
		try
		{
			auto entity_handler = script::EntityScriptHandler(get_owner());

			function.call(entity_handler);
		}
		catch (LuaIntf::LuaException& ex)
		{
			LOG(LOG_ERROR, "Entity [%s] Component [%s] throws lua exception: %s", get_owner()->get_type().c_str(), "Script", ex.what());
		}
	}
}

bool game::component::ScriptHandler::is_function(const LuaIntf::LuaRef& function)
{
	return function.isValid() && function.isFunction();
}