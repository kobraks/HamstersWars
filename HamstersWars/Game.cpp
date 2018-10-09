// Darren brown the heist 1,2,3,4
#include "stdafx.h"
#include "Game.h"
#include <GL/glew.h>

#include "GlewException.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Model.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "Mesh.h"
#include "Script.h"
#include "Timer.h"
#include "EntityScriptHandler.h"
#include "SceneManager.h"
#include "ConsoleArgumentContainer.h"
#include "ConfigurationLoader.h"

#define ADD_FUNCTION(TYPE, X) addFunction(#X, TYPE::X)
#define ADD_FUNCTION_PARAMS(TYPE, X, PARAMS) addFunction(#X, TYPE::X, PARAMS)

glm::vec3 pos(0.f, 1.f, 4.f);
glm::vec3 dir (0.f, 0.f, -1.f);
glm::vec3 up(0.f, 1.f, 0.f);

const float speed = 0.05f;
bool flyingMode = true;
bool captureMouse = false;

float velocity_vertical;
float velocity_horizontal;

float window_height, window_width;
std::shared_ptr<game::model::Model> modela;

void update()
{
	pos.x += dir.x * speed * velocity_vertical;
	pos.z += dir.z * speed * velocity_vertical;

	pos.x += dir.z * speed * velocity_horizontal;
	pos.z -= dir.x * speed * velocity_horizontal;

	if (flyingMode)
		pos.y += dir.y * speed * velocity_vertical;
	else
		pos.y = 0.5;

	velocity_vertical /= 1.2;
	velocity_horizontal /= 1.2;
}

void print_log(const int& log_level, const std::string& message)
{
	LOG(static_cast<LogLevel::TLogLevel>(log_level), message.c_str());
}

namespace game
{
	bool is_running()
	{
		return Game::get_instance()->is_running();
	}

	void quit(int exit_code)
	{
		Game::get_instance()->quit(exit_code);
	}

	Game* game = nullptr;

	Game::Game()
	{
		game = this;
	}

	Game::~Game()
	{
	}

	Game* Game::get_instance()
	{
		return game;
	}

	void Game::process_arguments(int argc, char** argv)
	{
		LOG(LOG_INFO, "Processing program arguments");
		ConsoleArgumentContainer args;

		if (args.contains("debug"))
		{
			LOG(LOG_INFO, "Debug mode enable");
			properties.add<bool>("Debug", args.get_argument("debug").value<bool>());
		}

		if (args.contains("config_file"))
		{
			LOG(LOG_INFO, "Detected external config_file");
			properties.add<std::string>("config_file", args.get_argument("config_file").value<std::string>());
		}
		else
			properties.add<std::string>("config_file", "../config.lua");
	}

	void Game::quit(const int& exit_code)
	{
		running_ = false;
		exit_code_ = exit_code;
	}

	int Game::run()
	{
		running_ = true;

		Keyboard::initialize(window_);
		Mouse::initialize(window_, true, false);

		register_classes();

		ConfigurationLoader::load_config(properties.get<std::string>("config_file"), properties);

		main_loop();

		return exit_code_;
	}

	bool Game::is_running() const
	{
		return running_;
	}

	void Game::register_classes()
	{
		REGISTER_CLASS(Timer);
		REGISTER_FUNCTION(gl::Vector3D, [](LuaIntf::LuaBinding& binding)
		{
			LOG(LOG_INFO, "Registering Vector3D");
			binding.
				beginClass<gl::Vector3D>("Vector3D").
					addConstructor(LUA_ARGS(float&, float&, float&)).
					addVariable("x", &gl::Vector3D::x, true).
					addVariable("y", &gl::Vector3D::y, true).
					addVariable("z", &gl::Vector3D::z, true).
				endClass();
		});
		REGISTER_CLASS_CONSTRUCTOR(game::script::EntityScriptHandler, nullptr);
		REGISTER_FUNCTION(FileLog, [](LuaIntf::LuaBinding& binding)
		{
			LOG(LOG_INFO, "Registering log");
			binding.
				beginModule("log").
					addConstant("error", LOG_ERROR).
					addConstant("warning", LOG_WARNING).
					addConstant("info", LOG_INFO).
					addConstant("debug", LOG_DEBUG).
					addConstant("debug1", LOG_DEBUG1).
					addConstant("debug2", LOG_DEBUG2).
					addConstant("debug3", LOG_DEBUG3).
					addConstant("debug4", LOG_DEBUG4).
					addFunction("print", &print_log, LUA_ARGS(int, std::string)).
				endModule();
			binding.
				beginModule("Log").
					addConstant("error", LOG_ERROR).
					addConstant("warning", LOG_WARNING).
					addConstant("info", LOG_INFO).
					addConstant("debug", LOG_DEBUG).
					addConstant("debug1", LOG_DEBUG1).
					addConstant("debug2", LOG_DEBUG2).
					addConstant("debug3", LOG_DEBUG3).
					addConstant("debug4", LOG_DEBUG4).
					addFunction("print", &print_log, LUA_ARGS(int, std::string)).
				endModule();
		});

		REGISTER_FUNCTION(Game, [](LuaIntf::LuaBinding& binding)
		{
			binding.
				beginModule("program").
					addFunction("quit", &game::quit).
					addFunction("is_running", &game::is_running).
					beginModule("status").
						addConstant("game_missing_asset", status_game_missing_asset).
						addConstant("game_stack_empty", status_game_stack_empty).
						addConstant("game_init_failed", status_game_init_failed).
						addConstant("error", status_error).
						addConstant("game_ok", status_game_ok).
						addConstant("no_error", status_no_error).
						addConstant("false", status_false).
						addConstant("true", status_true).
						addConstant("ok", status_ok).
					endModule().
				endModule();
		});
	}
}
