#pragma once
#include <vector>
#include "PropertyManager.h"

/*
		if (!properties.contains("debug"))
			properties.add<bool>("debug", false);

		properties.add<sf::Keyboard::Key>("up", sf::Keyboard::W);
		properties.add<sf::Keyboard::Key>("down", sf::Keyboard::S);
		properties.add<sf::Keyboard::Key>("left", sf::Keyboard::A);
		properties.add<sf::Keyboard::Key>("right", sf::Keyboard::D);
 */

namespace gl
{
	class Program;
}

namespace game
{
	class Game;
	class ConfigurationLoader;

	class Configuration
	{
	public:
		typedef std::vector<gl::Program*> shader_list_type;

		struct WindowSettings
		{
			std::string title;
			sf::Vector2u size;
			sf::Vector2u position;
			uint32 style;
		};

		struct OpenGlSettings
		{
			uint32 major_version;
			uint32 minor_version;
			uint32 depth_bits;
			uint32 stencil_bits;
		};

		Configuration();
		Configuration(const Configuration& config);

		Configuration& operator=(const Configuration& config);

		WindowSettings window_settings;
		OpenGlSettings open_gl_settings;
		shader_list_type shader_list;

		PropertyManager properties;

	private:
		uint32 max_updates = 5;
		int32 update_rate = static_cast<uint32>(1000.0f / 60.0f);
		bool debug = false;

		friend Game;
		friend ConfigurationLoader;
	};
}

