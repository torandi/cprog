#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "game.hpp"
#include "config.hpp"
#include "character.hpp"
#include "area.hpp"
#include "logging.hpp"

#include <algorithm>

namespace game {
	std::default_random_engine Game::generator;
	std::vector<std::uniform_int_distribution<int> > Game::dices;

	Game * Game::singleton = nullptr;

	Game::Game() {

		dices.push_back(std::uniform_int_distribution<int>(1,5));
		dices.push_back(std::uniform_int_distribution<int>(1,6));
		dices.push_back(std::uniform_int_distribution<int>(1,10));
		dices.push_back(std::uniform_int_distribution<int>(1,20));


		/* Load world */
		Config game_config = Config::from_filename("game/game.yaml");
		//Config item_config = Config::from_filename("game/items.yaml");
		Config areas_config = Config::from_filename("game/areas.yaml");

		for(const ConfigNode * node : areas_config.root().list()) {
			Area * area = nullptr;
			if(node->tag() == "!area") {
				area = Area::from_config(node);
			} else {
				node->print();
				Logging::fatal("Invalid or missing tag for area");
			}
			areas[(*node)["/id"].parse_string()] = area;
		}


		//Fix area exits
		for(const ConfigNode * node : areas_config.root().list()) {
			Area * area = areas.find((*node)["/id"].parse_string())->second;
			const ConfigNode * exits_node = node->find("/exits");
			if(exits_node) {
				std::map<std::string, Area*> exits;
				for(auto &exit : exits_node->map()) {
					auto e_area = areas.find(exit.second->parse_string());
					if(e_area == areas.end()) {
						Logging::fatal("Unknown area id %s for exit %s from area %s\n", exit.second->parse_string().c_str(), exit.first.c_str(), area->name().c_str());
					}
					exits[exit.first] = e_area->second;
				}
				area->set_exits(exits);
			}
		}

	}

	Game::~Game() {
		for(auto &a : areas) { delete a.second; }
		std::for_each(characters.begin(), characters.end(), [](Character * c) { delete c; });
	}

	int Game::roll_dice(Game::dice_t dice, int op) {
		int value = dices[dice](generator);
		if(op != -1 && value <= op) {
			return value + roll_dice(dice, op);
		} else {
			return value;
		}
	}

	Game::try_result_t Game::try_action(int points) {
		int roll = roll_dice(T20);
		if(roll == 1) return PERFECT;
		else if(roll == 20) return FATAL;
		else if(roll <= points) return SUCCESS;
		else return FAIL;
	}

	std::ostream &Game::output(const Area* area) {
		return std::cout;
	}

	std::ostream &Game::out(const Area* area) {
		return Game::singleton->output(area);
	}
}
