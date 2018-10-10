#include "stdafx.h"
#include "ConfigurationLoader.h"
#include "Script.h"

#include "Program.h"
#include "Shader.h"

#define UNKNOWN_VALUE(cell) LOG(LOG_WARNING, "Unknown value: %s", cell.key<std::string>().c_str());

namespace game
{
	Configuration ConfigurationLoader::load_config(const std::string& file_name)
	{
		Configuration configuration;

		try
		{
			lua::Script::do_file(file_name);
			parse(LuaIntf::LuaRef::popFromStack(lua::Script::lua()), configuration);
		}
		catch(std::exception& ex)
		{
			LOG(LOG_ERROR, "Unable to load configuration from %s, error message: %s", file_name.c_str(), ex.what());
		}

		return configuration;
	}

	void ConfigurationLoader::parse(LuaIntf::LuaRef table, Configuration& configuration)
	{
		for (auto cell : table)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(cell.key<std::string>()));

			if (key == "WINDOW")
				parse_window(cell.value(), configuration);
			else if (key == "SHADER")
				parse_shader(cell.value(), configuration);
			else if (key == "OPEN_GL")
				parse_open_gl(cell.value(), configuration);
			else
			{
				LOG(LOG_INFO, "Parsing %s", cell.key<std::string>().c_str());
				parse_other(cell.value(), key, configuration);
			}
		}
	}

	void ConfigurationLoader::parse_shader(LuaIntf::LuaRef shader_table, Configuration& configuration)
	{
		LOG(LOG_INFO, "Parsing shader table");

		if (!shader_table.isTable())
		{
			LOG(LOG_ERROR, "Shader table is not table");
			return;
		}

		std::string shader_name;

		for (const auto& value : shader_table)
		{
			try
			{
				auto shader = load_program(value.value<std::string>());
				configuration.shader_list.push_back(shader);
			}
			catch(std::exception& ex)
			{
				LOG(LOG_ERROR, "Unable to load shader: %s=%s, %s", 
					value.key<std::string>().c_str(), value.value<std::string>().c_str(), ex.what());
			}
		}
	}

	void ConfigurationLoader::parse_window(LuaIntf::LuaRef window_table, Configuration& configuration)
	{
		LOG(LOG_INFO, "Parsing window configuration");

		if (!window_table.isTable())
		{
			LOG(LOG_ERROR, "No window configuration");
			return;
		}

		for (const auto& cell : window_table)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(cell.key<std::string>()));
			
			if (key == "TITLE")
			{
				LOG(LOG_INFO, "WIndow title: %s", cell.value<std::string>().c_str());
				configuration.window_settings.title = cell.value<std::string>();
			}
			else if (key == "SIZE")
			{
				auto size = get_window_size(cell.value());

				LOG(LOG_INFO, "Window size(%ux%u)", size.x, size.y);
				configuration.window_settings.size = size;
			}
			else if (key == "POSITION")
			{
				auto position = get_window_position(cell.value());

				LOG(LOG_INFO, "Window pozition(%ux%u)", position.x, position.y);
				configuration.window_settings.position = position;
			}
			else if (key == "STYLE")
			{
				LOG(LOG_INFO, "Window style: %i", cell.value<uint32>());
				configuration.window_settings.style = cell.value<uint32>();
			}
			else
				UNKNOWN_VALUE(cell);
		}
	}

	void ConfigurationLoader::parse_other(LuaIntf::LuaRef value, const std::string& key, Configuration& configuration)
	{
		if (!value.isTable() && !value.isFunction() && value.isValid())
		{
			if (key == "DEBUG")
			{
				LOG(LOG_INFO, "Develop mode: %", value.toValue<bool>() ? "true" : "false");
				configuration.debug = value.toValue<bool>();
			}
			else if (key == "MAX_UPDATES")
			{
				configuration.max_updates = value.toValue<uint32>();
				LOG(LOG_INFO, "Setting max updates to: %u", value.toValue<uint32>());
			}
			else if (key == "UPDATE_RATE")
			{
				configuration.update_rate = value.toValue<int32>();
				LOG(LOG_INFO, "Setting update rate to: %u", value.toValue<int32>());
			}
			else
			{
				configuration.properties.add<LuaIntf::LuaRef>(key, value);
				LOG(LOG_INFO, "Adding properties");
			}
		}
	}

	void ConfigurationLoader::parse_open_gl(LuaIntf::LuaRef table, Configuration& configuration)
	{
		LOG(LOG_INFO, "Parsing open gl settings");

		for (const auto& cell : table)
		{
			auto key = utils::to_upper_copy(utils::trim_copy(cell.key<std::string>()));
			if (key == "VERSION")
			{
				auto version = get_version(cell.value());
				configuration.open_gl_settings.major_version = version.first;
				configuration.open_gl_settings.minor_version = version.second;

				LOG(LOG_INFO, "open gl version: %u,%u", version.first, version.second);
			}
			else if (key == "DEPTH_BITS")
			{
				configuration.open_gl_settings.depth_bits = cell.value<uint32>();
				LOG(LOG_INFO, "depth bits: %u", cell.value<uint32>());
			}
			else if (key == "STENCIL_BITS")
			{
				configuration.open_gl_settings.stencil_bits = cell.value<uint32>();
				LOG(LOG_INFO, "stencil bits: %u", cell.value<uint32>());
			}
			else
				UNKNOWN_VALUE(cell);
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

	std::pair<uint32, uint32> ConfigurationLoader::get_version(LuaIntf::LuaRef version)
	{
		std::pair<uint32, uint32> result = {3, 3};

		try
		{
			for (auto cell : version)
			{
				auto key = utils::to_upper_copy(utils::trim_copy(cell.key<std::string>()));

				if (key == "1" || key == "MAJOR")
					result.first = cell.value<uint32>();
				if (key == "2" || key == "MINOR")
					result.second = cell.value<uint32>();
				else
					UNKNOWN_VALUE(cell);
			}
		}
		catch(std::exception& ex)
		{
			LOG(LOG_ERROR, "Unable to read open gl version, %s", ex.what());
		}

		return result;
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
