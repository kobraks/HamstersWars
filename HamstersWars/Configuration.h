#pragma once
#include <vector>
#include "PropertyManager.h"

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
		typedef std::vector<std::string> shader_path_list_type;

		struct WindowSettings
		{
			std::string title;
			sf::Vector2u size;
			sf::Vector2u position;
			uint32 style;
		};

		/*
		struct OpenGlSettings
		{
			uint32 major_version;
			uint32 minor_version;
			uint32 depth_bits;
			uint32 stencil_bits;
		};
		*/

		Configuration();
		Configuration(const Configuration& config);
		~Configuration();

		Configuration& operator=(const Configuration& config);

		WindowSettings window_settings;
		sf::ContextSettings context_settings;
		shader_list_type shader_list;

		PropertyManager properties;

	private:
		std::string entities_list_file;
		uint32 max_updates = 5;
		int32 update_rate = static_cast<uint32>(1000.0f / 60.0f);
		bool debug = false;
		shader_path_list_type shader_path_list;

		void load_shaders();

		friend Game;
		friend ConfigurationLoader;
	};
}

