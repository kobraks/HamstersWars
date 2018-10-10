#include "stdafx.h"
#include "Configuration.h"


namespace game
{
	Configuration::Configuration()
	{
		window_settings = { "EMPTY", sf::Vector2u(800, 600), sf::Vector2u(100, 100), sf::Style::Default };
		open_gl_settings = { 3, 3, 24, 8 };
	}

	Configuration::Configuration(const Configuration& config)
	{
		window_settings = config.window_settings;
		open_gl_settings = config.open_gl_settings;
		max_updates = config.max_updates;
		update_rate = config.update_rate;
		debug = config.debug;

		properties = config.properties;
	}

	Configuration& Configuration::operator=(const Configuration& config)
	{
		window_settings = config.window_settings;
		open_gl_settings = config.open_gl_settings;
		max_updates = config.max_updates;
		update_rate = config.update_rate;
		debug = config.debug;

		properties.clone(config.properties);
		return *this;
	}
}
