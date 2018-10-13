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

	void print_table(LuaIntf::LuaRef table)
	{
		if (table.isTable())
			for (auto cell : table)
			{
				if (cell.value().isTable())
				{
					LOG(LOG_DEBUG4, "%s = table", cell.key<std::string>().c_str());
					print_table(cell.value());
				}
				else
					LOG(LOG_DEBUG4, "%s", cell.key<std::string>().c_str());
			}
	}

	void ConfigurationLoader::parse(LuaIntf::LuaRef table, Configuration& configuration)
	{
		print_table(table);

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
			auto path = value.value<std::string>();
			if (utils::is_local_path(path))
				path.insert(0, SHADERS_PATH);

			LOG(LOG_INFO, "shader path: %s", path.c_str())
			configuration.shader_path_list.push_back(path);
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

				LOG(LOG_INFO, "Window size (%ux%u)", size.x, size.y);
				configuration.window_settings.size = size;
			}
			else if (key == "POSITION")
			{
				auto position = get_window_position(cell.value());

				LOG(LOG_INFO, "Window position(%ux%u)", position.x, position.y);
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
			else if (key == "ENTITY_LIST_FILE")
			{
				auto path = value.toValue<std::string>();

				if (utils::is_local_path(path))
					path.insert(0, LUA_SCRIPTS_PATH);

				configuration.entities_list_file = path;
				LOG(LOG_INFO, "Setting entity list file to: %s", path.c_str());
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
				configuration.context_settings.majorVersion = version.first;
				configuration.context_settings.minorVersion = version.second;

				LOG(LOG_INFO, "open gl version: %u,%u", version.first, version.second);
			}
			else if (key == "DEPTH_BITS")
			{
				configuration.context_settings.depthBits = cell.value<uint32>();
				LOG(LOG_INFO, "depth bits: %u", cell.value<uint32>());
			}
			else if (key == "STENCIL_BITS")
			{
				configuration.context_settings.stencilBits = cell.value<uint32>();
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

		if (window_position.isValid())
		{
			if (window_position.isTable())
			{
				for (auto element : window_position)
				{
					try
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
					catch (LuaIntf::LuaException& ex)
					{
						LOG(LOG_WARNING, "Unable to parse Lua table %s", ex.what());
					}
				}
			}
			else if (!window_position.isFunction())
			{
				try
				{
					switch(window_position.toValue<uint>())
					{
					case 1:
						LOG(LOG_INFO, "position center");
						position.x = sf::VideoMode::getDesktopMode().width / 2;
						position.y = sf::VideoMode::getDesktopMode().height / 2;
						break;
					default:
						LOG(LOG_WARNING, "Invalid value");
					}
				}
				catch (LuaIntf::LuaException& ex)
				{
					LOG(LOG_WARNING, "Unable to parse Lua table %s", ex.what());
				}

			}
		}
		else
			LOG(LOG_WARNING, "Lua variable is not valid");

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
}
