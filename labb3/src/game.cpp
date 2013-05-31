#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "game.hpp"
#include "world_parser.hpp"
#include "character.hpp"
#include "area.hpp"
#include "logging.hpp"
#include "player.hpp"

#include <algorithm>
#include <chrono>
#include <random>

namespace game {
	std::default_random_engine Game::generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::vector<std::uniform_int_distribution<int> > Game::dices;

	Game * Game::singleton = nullptr;

	Game::Game() {

		dices.push_back(std::uniform_int_distribution<int>(1,5));
		dices.push_back(std::uniform_int_distribution<int>(1,6));
		dices.push_back(std::uniform_int_distribution<int>(1,10));
		dices.push_back(std::uniform_int_distribution<int>(1,20));

		WorldParser::parse(this);
    for(auto it : areas) {
      Area * a = it.second;
      std::cerr << a->name() << ": " << a->description() << std::endl;
      std::cerr << "Items:" << std::endl;
      for(Item * i : a->items() ) {
        std::cerr << i->name() << ": " << i->description() << std::endl;
      }
			std::cerr << std::endl;
			std::cerr << "Characters:" << std::endl;
			for(const Character * c : a->characters() ) {
				std::cout << c->name() << ": " << c->description() << std::endl;
			}
      std::cerr << std::endl;
			std::cerr << std::endl;
    }
	}

	void Game::stop() {
		m_run = false;
	}

	void Game::start_simulation() {
		using namespace std::placeholders;
		while(m_run) {
			//ROLL FOR INITIATIVE SUCKERS!
			std::for_each(characters.begin(), characters.end(), std::bind(&Character::roll_initiative, _1));
			std::sort(characters.begin(), characters.end(), [](Character * c1, Character * c2) { return c1->initiative() > c2->initiative(); } );

			for(Character * c : characters) {
				c->action();
			}
			std::cout << std::endl;
		}
	}

	Game::~Game() {
		for(auto &a : areas) { delete a.second; }
		std::for_each(characters.begin(), characters.end(), [](Character * c) { delete c; });
    delete m_player;
	}

	int Game::roll_dice(Game::dice_t dice, int op) {
		int value = dices[dice](generator);
		if(op != -1 && value >= op) {
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

  std::string Game::lowercase(const std::string &str) {
    std::string ret = str;
    std::transform(ret.begin(), ret.begin(), ret.end(), [](const char c) { return (char)std::tolower(c); });
    return ret;
  }

  Player * Game::player() {
    return Game::singleton->m_player;
  }

  Area * Game::area(const std::string &name) {
    auto it = areas.find(name);
    if(it != areas.end()) {
      return it->second;
    } else {
      return nullptr;
    }
  }

  Keepable * Game::item(const std::string &name) {
    auto it = items.find(name);
    if(it != items.end()) {
      return it->second;
    } else {
      return nullptr;
    }
  }

	void Game::add_character(Character * character) {
		characters.push_back(character);
		character->location()->enter(character);
	}
}
