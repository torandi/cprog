#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "character.hpp"
#include "game.hpp"
#include "area.hpp"
#include "item.hpp"

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

	void Character::regain_life(int life) {
		int l = std::min(life, m_max_life - m_life);
		Game::out(location()) << name() << " regains " << l << " hp." << std::endl;
		m_life += l;
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

	const Area* Character::location() const {
		return m_location;
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

	int Character::armor_protection() const {
		return m_armor / 10;
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
		return Game::try_action(cost + m_action_mod);
	}

	void Character::pick_up(Item * item) {
		if(m_location->pick_up(this, item)) {
			Game::out(location()) << name() << " picks up " << item->name() << std::endl;
			item->aquire(this);
		}
	}

}
