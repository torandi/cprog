#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "game.hpp"
#include "logging.hpp"

using namespace game;

int main(int argc, char* argv[]){
  Logging::init();
  Logging::add_destination(Logging::ERROR, stderr);
  Game::singleton = new Game();

  Logging::cleanup();
}
