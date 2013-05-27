#include "game.hpp"
#include "config.hpp"

namespace game {
	std::default_random_engine Game::generator;
	std::vector<std::uniform_int_distribution<int> > Game::dices;

	Game::Game() {

		dices.push_back(std::uniform_int_distribution<int>(1,5));
		dices.push_back(std::uniform_int_distribution<int>(1,6));
		dices.push_back(std::uniform_int_distribution<int>(1,10));
		dices.push_back(std::uniform_int_distribution<int>(1,20));

		Config config = Config::from_filename("game/game.yaml");
	}

	Game::~Game() {

	}

	int Game::roll_dice(Game::dice_t dice, int op) {
		int value = dices[dice](generator);
		if(op != -1 && value <= op) {
			return value + roll_dice(dice, op);
		} else {
			return value;
		}
	}
}
