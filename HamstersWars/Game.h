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

		void* window_;

		void draw();
		void on_reshape(int width, int height);
		void main_loop();
		void update(const float& time_step);

		static Game* get_instance();
	};
}
