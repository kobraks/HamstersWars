#pragma once

#include <SFML/System.hpp>
#include "Camera.h"
#include "PropertyManager.h"

namespace game
{
	class Game
	{
	public:
		enum status_type
		{
			status_game_missing_asset = -4,  ///< Application failed due to missing asset file
			status_game_stack_empty = -3,  ///< Application States stack is empty
			status_game_init_failed = -2,  ///< Application initialization failed
			status_error = -1,  ///< General error status response
			status_game_ok = 0,  ///< Application quit without error
			status_no_error = 0,  ///< General no error status response
			status_false = 0,  ///< False status response
			status_true = 1,  ///< True status response
			status_ok = 1   ///< OK status response
		};

		PropertyManager properties;

		Game();
		~Game();

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;


		static Game* get_instance();
		void process_arguments(int argc, char** argv);

		void quit(const int& exit_code = status_game_ok);
		int run();

		bool is_running() const;


	private:
		gl::Camera* camera_;
		bool running_ = false;
		int exit_code_ = 0;

		void* window_;
		void* scene_manager_;

		void draw();
		void on_reshape(int width, int height);
		void main_loop();
		void update(const float& time_step);

		static void register_classes();
	};
}
