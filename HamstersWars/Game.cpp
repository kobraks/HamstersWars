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

namespace game
{
	void print_log(const int& log_level, const std::string& message)
	{
		LOG(static_cast<log::level::TLogLevel>(log_level), message.c_str());
	}

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
			properties.add<std::string>("config_file", CONFIG_FILE);
	}

	void Game::quit(const int& exit_code)
	{
		running_ = false;
		exit_code_ = exit_code;
	}

	int Game::run()
	{
		running_ = true;

		Keyboard::initialize();
		Mouse::initialize();
		register_classes();

		auto config = ConfigurationLoader::load_config(properties.get<std::string>("config_file"));
		properties.add<Configuration>("config", config);

		set_max_updates(config.max_updates);
		set_update_rate(config.update_rate);
		properties.set<bool>("debug", config.debug);

		init_renderer();
		Mouse::set_window(window_);

		main_loop();

		LOG(LOG_INFO, "Exit code: %i", exit_code_);

		return exit_code_;
	}

	bool Game::is_running() const
	{
		return running_;
	}

	void Game::set_max_updates(const uint32& max_updates)
	{
		if (200 >= max_updates && 1 <= max_updates)
			max_updates_ = max_updates;
	}

	void Game::set_update_rate(const float& update_rate)
	{
		if (200.0f >= update_rate && 1.0f <= update_rate)
			update_rate_ = static_cast<uint32>(1000.0f / update_rate);
	}

	float Game::get_update_rate() const
	{
		return 1000.0f / static_cast<float>(update_rate_);
	}

	void Game::init_renderer()
	{
		auto config = properties.get<Configuration>("config");
		sf::ContextSettings context_settings;
		context_settings.depthBits = config.open_gl_settings.depth_bits;
		context_settings.stencilBits = config.open_gl_settings.stencil_bits;
		context_settings.majorVersion = config.open_gl_settings.major_version;
		context_settings.minorVersion = config.open_gl_settings.minor_version;

		auto window = new sf::Window(sf::VideoMode(config.window_settings.size.x, config.window_settings.size.y),
		                             config.window_settings.title, config.window_settings.style, context_settings);
		window->setPosition(sf::Vector2i(config.window_settings.position.x, config.window_settings.position.y));

		window_ = window;
	}

	void Game::draw()
	{
		auto window = reinterpret_cast<sf::Window*>(window_);
		window->setActive();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glFlush();
		window->display();
	}

	void Game::on_reshape(int width, int height)
	{
		auto window = reinterpret_cast<sf::Window*>(window_);
		window->setActive();
		glViewport(0, 0, width, height);
		camera_->set_width(width);
		camera_->set_height(height);

		/*shader_->use();
		shader_->get_parameter("Projection").set_value(camera_->get_projection());*/
	}

	void Game::update()
	{
		if (properties.get<bool>("debug"))
		{
			if (Keyboard::is_up(sf::Keyboard::T))
			{
				if (captureMouse)
				{
					captureMouse = false;
					Mouse::set_visable(true);
					Mouse::set_grabbed(false);
				}
				else
				{
					captureMouse = true;
					Mouse::set_visable(false);
					Mouse::set_grabbed(true);
				}
			}
		}
	}

	void Game::main_loop()
	{
		sf::Clock update_clock;
		sf::Clock frame_clock;

		auto window = reinterpret_cast<sf::Window*>(window_);
		sf::Event event;
		int32 elapsed_time = 0;
		update_clock.restart();

		while(is_running() && window && window->isOpen())
		{
			while(window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();

				if (event.type == sf::Event::Resized)
				{
					auto size = window->getSize();
					on_reshape(size.x, size.y);
				}

				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
					Keyboard::parse_event(event);

				if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.
					type == sf::Event::MouseEntered || event.type == sf::Event::MouseLeft || event.type == sf::Event::
					MouseMoved || event.type == sf::Event::MouseWheelMoved || event.type == sf::Event::MouseWheelScrolled)
					Mouse::parse_event(event);
			}

			int32 updates = 0;

			elapsed_time += update_clock.restart().asMilliseconds();
			while(elapsed_time >= update_rate_ && updates++ < max_updates_)
			{
				update();

				elapsed_time -= update_rate_;
			}

			draw();
		}
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
		REGISTER_FUNCTION(log::Log, [](LuaIntf::LuaBinding& binding)
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
				endModule().
				beginModule("window").
					addConstant("none", sf::Style::None).
					addConstant("titlebar", sf::Style::Titlebar).
					addConstant("resize", sf::Style::Resize).
					addConstant("close", sf::Style::Close).
					addConstant("fullscreen", sf::Style::Fullscreen).
					addConstant("default", sf::Style::Default).
				endModule();
		});
	}
}
