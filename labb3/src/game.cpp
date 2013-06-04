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
#include "input.hpp"

#include <algorithm>
#include <chrono>
#include <random>

namespace game {
	std::default_random_engine Game::generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::vector<std::uniform_int_distribution<int> > Game::dices;

	Game * Game::singleton = nullptr;

	Game::Game() {

		dices.push_back(std::uniform_int_distribution<int>(1,5));
		dices.push_back(std::uniform_int_distribution<int>(1,6));
		dices.push_back(std::uniform_int_distribution<int>(1,10));
		dices.push_back(std::uniform_int_distribution<int>(1,20));
	}
	void Game::init() {
		WorldParser::parse(this);
	}

	void Game::stop() {
		m_run = false;
	}

	void Game::start_simulation() {
		using namespace std::placeholders;

    std::cout << std::endl << game_name << std::endl;
    std::cout << " --------------------- " << std::endl << std::endl;
    std::cout << intro << std::endl;
    std::cout << " --------------------- " << std::endl << std::endl;
    Input::describe_area();
    std::cout << std::endl;

		while(m_run) {
			std::cout << "--" << std::endl << std::endl;
			//ROLL FOR INITIATIVE SUCKERS!

			Logging::verbose("Round start\n");

			std::for_each(characters.begin(), characters.end(), std::bind(&Character::roll_initiative, _1));
			std::sort(characters.begin(), characters.end(),
          [](Character * c1, Character * c2) { return c1->initiative() > c2->initiative(); } );

			std::for_each(characters.begin(), characters.end(), std::bind(&Character::init_round, _1));

			for(Character * c : characters) {
				Logging::verbose("%s, initiative: %d\n", c->name().c_str(), c->initiative());
				if(c->state() != Character::DEAD) c->action();
        if(!m_run) break;
			}

			std::cout << std::endl;
		}

    std::cout << " --------------------- " << std::endl << std::endl;
    std::cout << "Thanks for playing " << game_name << ". The game was created by " << author << ", and the engine written by Torandi." << std::endl;
	}

	Game::~Game() {
		for(auto &a : areas) { delete a.second; }
		std::for_each(characters.begin(), characters.end(), [](Character * c) { delete c; });
    delete m_player;
	}

	int Game::rnd(int min, int max) {
		std::uniform_int_distribution<int> rng(min, max);
		return rng(generator);
	}

	std::default_random_engine & Game::rng() {
		return generator;
	}

	void Game::set_rng_seed(long seed) {
		generator.seed(seed);
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
		Logging::verbose("ROLL: %d vs %d\n", roll, points);
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
		if(Game::singleton == nullptr) return cnull;
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

  void Game::character_dies(Character * character) {
    if(!m_won && final_monster != nullptr && character == final_monster) {
      std::cout << std::endl << end_text << std::endl;
      m_won = true;
      //m_run = false;
    }
  }

	void Game::add_character(Character * character) {
		characters.push_back(character);
		character->location()->m_characters.insert(character);
    if(character->name() == final_monster_name) {
      final_monster = character;
    }
	}

	void Game::save(const std::string &filename) {

	}

	void Game::load(const std::string &filename) {

	}
}
