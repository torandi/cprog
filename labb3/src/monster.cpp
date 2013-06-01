#include "monster.hpp"
#include "area.hpp"
#include "item.hpp"
#include "keepable.hpp"

#include <algorithm>

namespace game {

	Monster::Monster(const std::string &name, const std::string &description, std::map<std::string, int> attributes, Area * location)
		: Character(name, description, Character::MONSTERS, attributes, location) {
	}

	Monster * Monster::from_config(const ConfigNode * node, Area * location) {
		std::string name = (*node)["/name"].parse_string();
		std::string description = (*node)["/description"].parse_string();

		std::map<std::string, int> attributes = Character::parse_attributes(&(*node)["/attributes"]);

		Monster * monster = new Monster(name, description, attributes, location);
		Character::parse_inventory(monster, node->find("/loot", false));
		return monster;
	}

  Monster::~Monster() {
    std::for_each(m_loot.begin(), m_loot.end(), [](Keepable * i) { delete i; });
  }

  void Monster::action() {
		try {
			switch(m_state) {
				case IDLE:
					/* TODO: some walk cycle */
					for(Character * c : location()->characters()) {
						if(c != this && faction_standings[faction()][c->faction()]) {
							attack(c, std::min(15, m_action_points));
							if(m_action_points > 0) action();
							break;
						}
					}
					break;
				case IN_FIGHT:
					if(m_action_points == 0) {
						Game::out(location()) << name() << " " << verb("do") << " nothing." << std::endl;
					}
					if(location()->contains_character(m_in_fight)) {
						while(m_action_points > 0) {
							attack(m_in_fight, std::min(15, m_action_points));
							if(m_in_fight == nullptr) return;
						}
					}
					break;
				default:
					break;
			}
		} catch (const char * c) { } /* no points left */
  }

	void Monster::store(Keepable * item) {
		m_loot.push_back(item);
	}

	void Monster::attack(Character * character, int points) {
		if(character->location() != location()) {
			Game::out(location()) << name() << " can't attack " << character->name() << "; not in same location." << std::endl;
			return;
		}

		roll_attack(Game::T10, points, character,
				attribute("damage_overpower", -1),
				attribute("damage_extra")
			);
	}

	void Monster::reduce_armor(int amount) {
		return; /* Monster's armor don't decay (natural armor) */
	}

	void Monster::die() {
		Game::out(location()) << name() << " dies in a pool of blood." << std::endl;
    for(auto it = m_loot.begin(); it != m_loot.end(); ++it) {
      if(location()->drop(this, *it)) {
        Game::out(location()) << name() << " " << verb("drop") << " " << (*it)->name() << std::endl;
        m_loot.erase(it);
      }
		}
	}

}
