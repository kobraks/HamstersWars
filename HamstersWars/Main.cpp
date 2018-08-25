#include <cstdlib>
#include <exception>

#include "Game.h"
#include "Log.h"

int main(int argc, char** argv)
{
	try
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

	return EXIT_SUCCESS;
}
