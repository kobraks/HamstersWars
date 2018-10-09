#include "stdafx.h"
#include "ConfigurationLoader.h"
#include "Script.h"

#include "Program.h"
#include "Shader.h"

namespace game
{
	void ConfigurationLoader::load_config(const std::string& file_name, PropertyManager& properties)
	{
		properties.add<std::string>("window_title", "EMPTY");
		properties.add<sf::Vector2u>("window_size", sf::Vector2u(800, 600));
		properties.add<sf::Vector2u>("window_position", sf::Vector2u(100, 100));

		if (!properties.contains("debug"))
			properties.add<bool>("debug", false);

		properties.add<sf::Keyboard::Key>("up", sf::Keyboard::W);
		properties.add<sf::Keyboard::Key>("down", sf::Keyboard::S);
		properties.add<sf::Keyboard::Key>("left", sf::Keyboard::A);
		properties.add<sf::Keyboard::Key>("right", sf::Keyboard::D);

		properties.add<uint32>("max_updates", 5);
		properties.add<int32>("update_rate", static_cast<uint32>(1000.0f / 60.0f));

		lua::Script::do_file(file_name);

		parse(LuaIntf::LuaRef::popFromStack(lua::Script::lua()), properties);
	}

	void ConfigurationLoader::parse(LuaIntf::LuaRef table, PropertyManager& properties)
	{
		for (auto element : table)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(element.key<std::string>()));

			if (key == "WINDOW")
				parse_window(element.value(), properties);
			else if (key == "SHADER")
				parse_shader(element.value(), properties);
			else
				parse_other(element.key(), key, properties);
		}
	}

	void ConfigurationLoader::parse_shader(LuaIntf::LuaRef shader_table, PropertyManager& properties)
	{
		LOG(LOG_INFO, "Parsing shader table");

		if (!shader_table.isTable())
		{
			LOG(LOG_ERROR, "Shader table is not table");
			return;
		}

		std::string shader_name;

		for (auto element : shader_table)
			properties.add<gl::Program*>(element.key<std::string>(), load_program(element.value<std::string>()));
	}

	void ConfigurationLoader::parse_window(LuaIntf::LuaRef window_table, PropertyManager& properties)
	{
		LOG(LOG_INFO, "Parsing window configuration");

		if (!window_table.isTable())
		{
			LOG(LOG_ERROR, "No window configuration");
			return;
		}

		for (auto element : window_table)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(element.key<std::string>()));
			
			if (key == "TITLE")
			{
				LOG(LOG_INFO, "WIndow title: %s", element.value<std::string>().c_str());
				properties.set<std::string>("window_title", element.value<std::string>());
			}
			else if (key == "SIZE")
			{
				auto size = get_window_size(element.value());

				LOG(LOG_INFO, "Window size(%ux%u)", size.x, size.y);
				properties.set<sf::Vector2u>("window_size", size);
			}
			else if (key == "POSITION")
			{
				auto position = get_window_position(element.value());

				LOG(LOG_INFO, "Window pozition(%ux%u)", position.x, position.y);
				properties.set<sf::Vector2u>("window_position", position);
			}
		}
	}

	void ConfigurationLoader::parse_other(LuaIntf::LuaRef lua, const std::string& key, PropertyManager& properties)
	{
		if (!lua.isTable() && !lua.isFunction() && lua.isValid())
		{
			if (key == "DEBUG")
			{
				LOG(LOG_INFO, "Develop mode: %", lua.toValue<bool>() ? "true" : "false");
				properties.set<bool>("debug", lua.toValue<bool>());
			}
			else if (key == "UP")
			{
				properties.set<sf::Keyboard::Key>("up", lua.toValue<sf::Keyboard::Key>());
				properties.set<sf::Keyboard::Key>("right", sf::Keyboard::D);
				LOG(LOG_INFO, "Up %i", static_cast<int>(lua.toValue<sf::Keyboard::Key>()));
			}
			else if (key == "DOWN")
			{
				properties.set<sf::Keyboard::Key>("down", lua.toValue<sf::Keyboard::Key>());
				LOG(LOG_INFO, "Down %i", static_cast<int>(lua.toValue<sf::Keyboard::Key>()));
			}
			else if (key == "LEFT")
			{
				properties.set<sf::Keyboard::Key>("left", lua.toValue<sf::Keyboard::Key>());
				LOG(LOG_INFO, "Left %i", static_cast<int>(lua.toValue<sf::Keyboard::Key>()));
			}
			else if (key == "RIGHT")
			{
				properties.set<sf::Keyboard::Key>("right", lua.toValue<sf::Keyboard::Key>());
				LOG(LOG_INFO, "Right %i", static_cast<int>(lua.toValue<sf::Keyboard::Key>()));
			}
			else if (key == "MAX_UPDATES")
			{
				properties.set<uint32>("max_updates", lua.toValue<uint32>());
				LOG(LOG_INFO, "Setting max updates to: %u", lua.toValue<uint32>());
			}
			else if (key == "UPDATE_RATE")
			{
				properties.set<int32>("max_updates", lua.toValue<int32>());
				LOG(LOG_INFO, "Setting max updates to: %u", lua.toValue<int32>());
			}
		}
	}

	sf::Vector2u ConfigurationLoader::get_window_size(LuaIntf::LuaRef window_size)
	{
		auto size = sf::Vector2u(800, 600);

		if (!window_size.isTable())
		{
			LOG(LOG_ERROR, "size is not table");
			return size;
		}


		for (auto element : window_size)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(element.key<std::string>()));

			if (key == "WIDTH" || key == "1")
				size.x = element.value<uint>();
			else if (key == "HEIGHT" || key == "2")
				size.y = element.value<uint>();
		}

		return size;
	}

	sf::Vector2u ConfigurationLoader::get_window_position(LuaIntf::LuaRef window_position)
	{
		auto position = sf::Vector2u(100, 100);

		if (!window_position.isTable())
		{
			LOG(LOG_ERROR, "position is not table");
			return position;
		}


		for (auto element : window_position)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(element.key<std::string>()));

			if (key == "X" || key == "1")
				position.x = element.value<uint>();
			else if (key == "Y" || key == "2")
				position.y = element.value<uint>();
			else if (key == "CENTER")
			{
				position.x = sf::VideoMode::getDesktopMode().width / 2;
				position.y = sf::VideoMode::getDesktopMode().height / 2;
			}
		}

		return position;
	}

	gl::Shader* ConfigurationLoader::load_shader(const std::string& file, const TShader& type)
	{
		gl::Shader* shader = nullptr;

		switch (type)
		{
		case vertex:
			LOG(LOG_DEBUG, "Loading vertex shader form: %s", file.c_str());
			shader = new gl::Shader(gl::shader_type::Vertex);
			break;
		case fragment:
			LOG(LOG_DEBUG, "Loading fragment shader form: %s", file.c_str());
			shader = new gl::Shader(gl::shader_type::Fragment);
			break;
		case geometry:
			LOG(LOG_DEBUG, "Loading geometry shader form: %s", file.c_str());
			shader = new gl::Shader(gl::shader_type::Geometry);
			break;
		}

		try
		{
			shader->load_source_form_file(file);
			LOG(LOG_DEBUG, "Compiling shader");
			shader->compile();
		}
		catch (gl::exception::CompileException& ex)
		{
			LOG(LOG_ERROR, "Unable to compile shader: %s", ex.what());
			delete shader;
			return nullptr;
		}
		catch (std::exception& ex)
		{
			LOG(LOG_ERROR, "Unable to read shader source: %s", ex.what());
			delete shader;
			return nullptr;
		}

		return shader;
	}

	gl::Program* ConfigurationLoader::generate_program(gl::Shader* vertex, gl::Shader* fragment, gl::Shader* geometry)
	{
		auto program = new gl::Program();

		if (vertex)
		{
			LOG(LOG_DEBUG, "Attaching vertex shader");
			program->attach(*vertex);
		}
		else
			LOG(LOG_WARNING, "No vertex shader");

		if (fragment)
		{
			LOG(LOG_DEBUG, "Attaching fragment shader");
			program->attach(*fragment);
		}
		else
			LOG(LOG_WARNING, "No fragment shader");

		if (geometry)
		{
			LOG(LOG_DEBUG, "Attaching geometry shader");
			program->attach(*geometry);
		}
		else
			LOG(LOG_WARNING, "No geometry shader");

		try
		{
			LOG(LOG_DEBUG, "Linking the shader");
			program->link();
		}
		catch (gl::exception::LinkException& ex)
		{
			LOG(LOG_ERROR, "Unable to link shaders %s", ex.what());

			delete program;
			program = nullptr;
		}

		return program;
	}

	gl::Program* ConfigurationLoader::load_program(const std::string& file)
	{
		gl::Shader* vertex_shader = load_shader(file + ".vert", vertex);
		gl::Shader* fragment_shader = load_shader(file + ".frag", fragment);
		gl::Shader* geometry_shader = load_shader(file + ".geo", geometry);

		gl::Program* program = generate_program(vertex_shader, fragment_shader, geometry_shader);

		delete vertex_shader;
		delete fragment_shader;
		delete geometry_shader;

		return program;
	}
}
