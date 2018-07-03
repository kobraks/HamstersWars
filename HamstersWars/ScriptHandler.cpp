#include "ScriptHandler.h"
#include "EntityScriptHandler.h"
#include "Script.h"

#define ADD_FUNCTION(x) addFunction(#x, &EntityScriptHandler::x)
#define ADD_FUNCTION_PARAMS(x,params) addFunction(#x, &EntityScriptHandler::x, params)

bool game::component::ScriptHandler::registered_ = false;

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner) : Component(owner)
{
	function_ = LuaIntf::LuaRef(Script::lua(), "update");

	if (!function_.isFunction())
		throw std::exception();

	register_functions(Script::lua());
}


game::component::Component* game::component::ScriptHandler::copy() const
{
	return nullptr;
}

void game::component::ScriptHandler::update()
{
	function_(script::EntityScriptHandler(get_owner()));
}

void game::component::ScriptHandler::register_functions(LuaIntf::LuaContext& context)
{ 
	if (registered_)
		registered_ = false;

	using namespace script;
	LuaIntf::LuaBinding(context).beginClass<glm::vec3>("vec3")
		.addConstructor(LUA_ARGS(float, float, float))
		.addVariable("x", &glm::vec3::x, true)
		.addVariable("y", &glm::vec3::y, true)
		.addVariable("z", &glm::vec3::z, true)
		.endClass()
		.beginClass<EntityScriptHandler>("EntityScriptHandler")
		.ADD_FUNCTION(get_elapsed_time)
		.ADD_FUNCTION_PARAMS(scale, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(translate, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(rotate, LUA_ARGS(float, glm::vec3))
		.ADD_FUNCTION(is_colliding)
		.ADD_FUNCTION(destroy)
		.endClass();
}

game::component::ScriptHandler::ScriptHandler(std::shared_ptr<Entity> owner, const std::string& code) : Component(owner)
{
	Script::lua().doString(code.c_str());

	function_ = LuaIntf::LuaRef(Script::lua(), "update");

	if (!function_.isFunction())
		throw std::exception();

	register_functions(Script::lua());
}
