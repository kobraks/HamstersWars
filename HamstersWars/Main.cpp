#include "stdafx.h"

#include "Game.h"

int main(int argc, char** argv)
{
	game::log::Log::add_output(new game::log::Output2File(LOG_FILE));
	game::log::Log::add_output(new game::log::Output2Console(stderr));

	auto game = new game::Game();
	game->process_arguments(argc, argv);

	const auto exit_code = game->run();
	delete game;

	return exit_code;

	/*try
	{
		game::Game::initialize(argc, argv, "Hamsters wars", sf::Vector2i(100, 100), 800, 600);
		game::Game::run();
	}
	catch(std::exception& ex)
	{
		game::Game::stop();
		game::Game::close();
		LOG(LOG_ERROR, "Game was closed with exception: %s", ex.what());

		system("pause");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;*/
}
