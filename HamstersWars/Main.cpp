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
}
