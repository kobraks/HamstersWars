#include "stdafx.h"
#include "Configuration.h"
#include "ShaderLoader.h"

namespace game
{
	Configuration::Configuration()
	{
		window_settings = { "EMPTY", sf::Vector2u(800, 600), sf::Vector2u(100, 100), sf::Style::Default };
		context_settings.depthBits = 24;
		context_settings.stencilBits = 8;
		context_settings.antialiasingLevel = 0; /// <- not supported
		context_settings.majorVersion = context_settings.minorVersion = 3;
	}

	Configuration::Configuration(const Configuration& config)
	{
		window_settings = config.window_settings;
		context_settings = config.context_settings;
		max_updates = config.max_updates;
		update_rate = config.update_rate;
		debug = config.debug;

		properties = config.properties;

		shader_list.reserve(config.shader_list.size());
		for (auto shader : config.shader_list)
			shader_list.push_back(new gl::Program(*shader));

		shader_path_list = config.shader_path_list;
	}

	Configuration::~Configuration()
	{
		for (auto shader : shader_list)
			delete shader;
	}

	Configuration& Configuration::operator=(const Configuration& config)
	{
		window_settings = config.window_settings;
		context_settings = config.context_settings;
		max_updates = config.max_updates;
		update_rate = config.update_rate;
		debug = config.debug;

		shader_list.reserve(config.shader_list.size());
		for (auto shader : config.shader_list)
			shader_list.push_back(new gl::Program(*shader));

		shader_path_list = config.shader_path_list;

		properties.clone(config.properties);
		return *this;
	}

	void Configuration::load_shaders()
	{
		shader_list.reserve(shader_path_list.size());
		for (auto path : shader_path_list)
			shader_list.push_back(ShaderLoader::load_program(path));
	}
}
