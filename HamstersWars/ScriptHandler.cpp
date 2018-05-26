#include "ScriptHandler.h"
#include "EntityScriptHandler.h"

#define ADD_FUNCTION(x) addFunction(#x, &EntityScriptHandler::x)
#define ADD_FUNCTION_PARAMS(x,params) addFunction(#x, &EntityScriptHandler::x, params)

game::component::ScriptHandler::ScriptHandler(game::Entity* owner, const std::shared_ptr<LuaIntf::LuaContext>& context, const LuaIntf::LuaRef& function) : context_(context), function_(function), Component(owner)
{
	if (!function_.isFunction())
		throw std::exception();

	register_functions();
}


game::component::ScriptHandler::~ScriptHandler()
{
}

void game::component::ScriptHandler::update()
{
	function_(script::EntityScriptHandler(get_owner()));
}

void game::component::ScriptHandler::register_functions()
{ 
	using namespace script;
	LuaIntf::LuaBinding(*context_).beginClass<glm::vec3>("vec3")
		.addVariable("x", &glm::vec3::x, true)
		.addVariable("y", &glm::vec3::y, true)
		.addVariable("z", &glm::vec3::z, true)
		.endClass()
		.beginClass<EntityScriptHandler>("EntityScriptHandler")
		.ADD_FUNCTION(get_elapsed_time)
		.ADD_FUNCTION_PARAMS(scale, LUA_ARGS(glm::vec3))
		.ADD_FUNCTION_PARAMS(translate, LUA_ARGS(float, float, float))
		.ADD_FUNCTION_PARAMS(rotate, LUA_ARGS(float, float, float, float))
		.ADD_FUNCTION(destroy)
		.endClass();
}

game::component::ScriptHandler::ScriptHandler(Entity* owner, const std::string& code) : Component(owner)
{
	context_->doString(code.c_str());
	function_ = LuaIntf::LuaRef(*context_, "update");

	if (!function_.isFunction())
		throw std::exception();

	register_functions();
}
