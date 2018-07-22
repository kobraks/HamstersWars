#include "ScriptHandler.h"
#include "EntityScriptHandler.h"
#include "Script.h"
#include "Log.h"

#define ADD_FUNCTION(x) addFunction(#x, &EntityScriptHandler::x)
#define ADD_FUNCTION_PARAMS(x,params) addFunction(#x, &EntityScriptHandler::x, params)

bool game::component::ScriptHandler::registered_ = false;

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner) : Component(owner)
{
	function_ = LuaIntf::LuaRef(lua::Script::lua(), "update");

	if (!function_.isFunction())
		throw std::exception();

	register_functions(lua::Script::lua());
}

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> entity, const LuaIntf::LuaRef& component_table) : Component(entity)
{
	function_ = component_table["update"];
	if (!function_.isFunction())
		throw std::exception();

	register_functions(lua::Script::lua());
}


game::component::Component* game::component::ScriptHandler::copy() const
{
	return nullptr;
}

void game::component::ScriptHandler::update()
{
	//function_(script::EntityScriptHandler(get_owner()));
	try
	{
		auto entity_handler = script::EntityScriptHandler(get_owner());

		function_.call(entity_handler);
		//function_.call(1);
	}
	catch(LuaIntf::LuaException& ex)
	{
		Log::level() = Log::log_error;
		Log::print("Entity [%s] Component [%s] throws lua exception: %s", get_owner()->get_type().c_str(), "Script", ex.what());
	}
}

void game::component::ScriptHandler::register_functions(LuaIntf::LuaContext& context)
{ 
	if (!registered_)
		registered_ = true;
	else return;

	using namespace script;
	auto binds = LuaIntf::LuaBinding(context);
	
	binds
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

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner, const std::string& code) : Component(owner)
{
	lua::Script::lua().doString(code.c_str());

	function_ = LuaIntf::LuaRef(lua::Script::lua(), "update");

	if (!function_.isFunction())
		throw std::exception();

	register_functions(lua::Script::lua());
}
