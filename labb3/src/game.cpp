#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "game.hpp"
#include "world_parser.hpp"
#include "character.hpp"
#include "area.hpp"
#include "logging.hpp"
#include "player.hpp"
#include "string_utils.hpp"

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

			std::for_each(characters.begin(), characters.end(), std::bind(&Character::init_round, _1));

			for(Character * c : characters) {
				if(c->state() != Character::DEAD) c->action();
        if(!m_run) break;
			}

			/* Perform death cleanup */
			for(auto it=characters.begin(); it!=characters.end(); ++it) {
				if((*it)->state() == Character::DEAD) {
					delete *it;
					it = characters.erase(it);
				}
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

	std::ostream &Game::output(const Area* area, const Area* area2) {
		if(area == m_player->location() || area2 == m_player->location()) return std::cout;
		else return cnull;
	}

	std::ostream &Game::out(const Area* area, const Area* area2) {
		return Game::singleton->output(area, area2);
	}

  std::string Game::lowercase(const std::string &str) {
    std::string ret = str;
    std::transform(ret.begin(), ret.end(), ret.begin(), [](const char c) { return (char)std::tolower(c); });
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
		character->location()->m_characters.insert(character);
	}
}
