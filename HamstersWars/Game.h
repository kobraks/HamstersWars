#pragma once

#include <fstream>
#include <SFML/System.hpp>
#include "Camera.h"
#include "Program.h"
#include "SceneManager.h"

namespace game
{
	class Game
	{
	public:
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		static void initialize(int argc, char** argv, const char* window_name, const sf::Vector2i& position, const int& widht, const int& height);
		static void run();

		static void stop();
		static void close();
	private:
		Game();
		~Game();

		std::fstream log_file_;
		gl::Camera* camera_;
		gl::Program* shader_;
		bool is_runing = false;

		void* window_;

		void draw();
		void on_reshape(int width, int height);
		void main_loop();
		void update(const float& time_step);

		enum TShader : int { fragment, geometry, vertex };

		static gl::Shader* load_shader(const std::string& file, const TShader& type);
		static gl::Program* generate_program(gl::Shader* vertex, gl::Shader* fragment, gl::Shader* geometry = nullptr);
		static gl::Program* load_program(const std::string& file);

		static Game* get_instance();
	};
}
