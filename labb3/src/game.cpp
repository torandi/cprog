#include "game.hpp"
#include "config.hpp"

namespace game {

	Game::Game() {
		Config config = Config::from_filename("game/game.yaml");
	}

	Game::~Game() {

	}

	void Game::change_faction_relation(const std::string &faction1, const std::string &faction2, int mod) const {

	}

	Game::relation_status_t Game::faction_relation(const std::string &faction1, const std::string &faction2) const {
		return NEUTRAL;
	}

}
