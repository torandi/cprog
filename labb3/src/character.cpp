#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "character.hpp"
#include "game.hpp"
#include "area.hpp"
#include "item.hpp"

namespace game {

	Character::Character(const std::string &name, const std::string &faction, const std::string & description, int life, std::map<std::string, int> attributes, Area * location)
		: m_max_life(life)
		, m_name(name)
		, m_faction(faction)
		, m_description(description)
		, m_attributes(attributes)
		, m_life(life)
		, m_location(location) {
	}

	Character::~Character() {

	}

	int Character::attribute(const std::string &attr) const {
		auto it = m_attributes.find(attr);
		if(it != m_attributes.end()) {
			return it->second;
		} else {
			return 0;
		}
	}

	void Character::new_turn() {
		m_action_points = attribute("action_points");
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

	const std::string &Character::faction() const {
		return m_faction;
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
		return attribute("initiative") + initiative_roll();
	}

	int Character::armor_protection() const {
		return attribute("armor") / 10;
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
			if(item->aquire(this)) {
				store(item);
			} else {
				delete item;
			}
		}
	}

}