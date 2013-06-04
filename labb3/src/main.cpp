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

#if ENABLE_DEBUG
	Logging::add_destination(Logging::VERBOSE, "game.log");
#endif

  Game::singleton = new Game();
	Game::singleton->init();
  Game::singleton->start_simulation();

	Input::cleanup();
  Logging::cleanup();
}
