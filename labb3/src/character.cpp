#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "character.hpp"
#include "game.hpp"
#include "area.hpp"

namespace game {

	Character::Character(const std::string &name, const std::string &type, const std::string & description, int life, int initiative, int action_points, Area * location)
		: m_max_life(life)
		, m_name(name)
		, m_type(type)
		, m_description(description)
		, m_initiative(initiative)
		, m_base_action_points(action_points)
		, m_life(life)
		, m_location(location) {
	}

	Character::~Character() {

	}

	void Character::new_turn() {
		m_action_points = m_base_action_points;
	}

	const int Character::life() const {
		return m_life;
	}

	const int Character::max_life() const {
		return m_max_life;
	}

	const std::string &Character::type() const {
		return m_type;
	}

	const std::string &Character::name() const {
		return m_name;
	}

	const std::string &Character::description() const {
		return m_description;
	}

	Character::state_t Character::state() const {
		return m_state;
	}

	void Character::go(const std::string &direction) {
		Area * new_location = m_location->neighbor(direction);
		if(new_location != nullptr) {
			m_location = new_location;
		}
	}

	const int Character::initiative() const {
		return m_initiative + initiative_roll();
	}

	void Character::fight(Character * character) {
		if(m_in_fight == nullptr) m_in_fight = character;
	}


	int Character::initiative_roll() const {
		return Game::roll_dice(Game::T10, 10);
	}

	void Character::do_action(int cost) {
		if(m_action_points >= cost) {
			m_action_points -= cost;
		} else {
			throw "More action points needed";
		}
	}

	Game::try_result_t Character::try_do_action(int cost) {
		do_action(cost);
		return Game::try_action(cost);
	}

}
