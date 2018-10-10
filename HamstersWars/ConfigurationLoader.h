#pragma once
#include "PropertyManager.h"
#include "Configuration.h"

namespace game
{
	class ConfigurationLoader
	{
	public:
		static Configuration load_config(const std::string& file_name);

	private:
		static void parse(LuaIntf::LuaRef table, Configuration& configuration);
		static void parse_shader(LuaIntf::LuaRef shader_table, Configuration& configuration);
		static void parse_window(LuaIntf::LuaRef window_table, Configuration& configuration);
		static void parse_other(LuaIntf::LuaRef value, const std::string& key, Configuration& configuration);
		static void parse_open_gl(LuaIntf::LuaRef table, Configuration& configuration);

		static sf::Vector2u get_window_size(LuaIntf::LuaRef window_size);
		static sf::Vector2u get_window_position(LuaIntf::LuaRef window_position);
		static std::pair<uint32, uint32> get_version(LuaIntf::LuaRef version);

		enum TShader : int { fragment, geometry, vertex };

		static gl::Shader* load_shader(const std::string& file, const TShader& type);
		static gl::Program* generate_program(gl::Shader* vertex, gl::Shader* fragment, gl::Shader* geometry = nullptr);
		static gl::Program* load_program(const std::string& file);
	};
}
