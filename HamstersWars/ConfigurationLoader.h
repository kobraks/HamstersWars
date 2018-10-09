#pragma once
#include "PropertyManager.h"

namespace game
{
	class ConfigurationLoader
	{
	public:
		static void load_config(const std::string& file_name, PropertyManager& properties);

	private:
		static void parse(LuaIntf::LuaRef table, PropertyManager& properties);
		static void parse_shader(LuaIntf::LuaRef shader_table, PropertyManager& properties);
		static void parse_window(LuaIntf::LuaRef window_table, PropertyManager& properties);
		static void parse_other(LuaIntf::LuaRef lua, const std::string& key, PropertyManager& properties);

		static sf::Vector2u get_window_size(LuaIntf::LuaRef window_size);
		static sf::Vector2u get_window_position(LuaIntf::LuaRef window_position);

		enum TShader : int { fragment, geometry, vertex };

		static gl::Shader* load_shader(const std::string& file, const TShader& type);
		static gl::Program* generate_program(gl::Shader* vertex, gl::Shader* fragment, gl::Shader* geometry = nullptr);
		static gl::Program* load_program(const std::string& file);
	};
}
