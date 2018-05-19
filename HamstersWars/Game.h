#pragma once

#include <fstream>
#include <SFML/System.hpp>

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

		void on_draw();
		void on_reshape(int width, int height);
		void on_timer(int id);

		static Game* get_instance();
		int window_handle_;
		
		friend void on_draw_callback();
		friend void on_reshape(int, int);
		friend void on_timer(int);
	};
}
