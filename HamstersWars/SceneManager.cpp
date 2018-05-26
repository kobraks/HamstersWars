#include "SceneManager.h"
#include "Lua/lua.hpp"
#include "lua/LuaIntf.h"

#include "Log.h"

game::SceneManager::SceneManager()
{
}

game::SceneManager::SceneManager(const gl::Program& program,
	shader_behavior behavior) : shader_(program), shader_behavior_(behavior)
{
}


game::SceneManager::~SceneManager()
{
}

void game::SceneManager::draw()
{
	shader_.use();

	for (auto model : models_)
	{
		if (shader_behavior_)
			shader_behavior_(shader_, model);
		else
			model->draw();
	}
}

void game::SceneManager::update()
{
}

void game::SceneManager::set_shader(const gl::Program& program)
{
	shader_ = program;
}

void game::SceneManager::set_shader_behavior(shader_behavior& behavior)
{
	shader_behavior_ = behavior;
}

void game::SceneManager::load_from_file(const std::string& file_name)
{
	LuaIntf::LuaContext lua;

	try
	{
		lua.doFile(file_name.c_str());
	}
	catch(LuaIntf::LuaException& ex)
	{
		Log::level() = Log::TLogLevel::log_error;
		Log::print("Lua exception: %s", ex.what());
	}

}
