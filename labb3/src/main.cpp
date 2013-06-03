#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "game.hpp"
#include "input.hpp"
#include "logging.hpp"

using namespace game;

int main(int argc, char* argv[]){
  Logging::init();
  Logging::add_destination(Logging::ERROR, stderr);
	Input::init();
	Logging::add_destination(Logging::VERBOSE, "game.log");

	if(argc > 1 && std::string(argv[1]) == "-v") {
		Logging::add_destination(Logging::VERBOSE, stderr);
	}
  Game::singleton = new Game();
	Game::singleton->init();
  Game::singleton->start_simulation();

	Input::cleanup();
  Logging::cleanup();
}
