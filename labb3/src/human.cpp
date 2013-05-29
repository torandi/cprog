#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "human.hpp"
#include "keepable.hpp"
#include "equipment.hpp"
#include "area.hpp"
#include "game.hpp"
#include "logging.hpp"

#include <algorithm>

namespace game {

	int Human::backpack_volume() const {
		return 5 + attribute("backpack_volume");
	}

	int Human::carrying_capacity() const {
		return 10 * ( 1 + attribute("strength") );
	}

	int Human::attribute(const std::string &attr) const {
		int val = Character::attribute(attr);
		for(const Equipment * e : m_equipments) {
			val += e->effect(attr);
		}
		return val;
	}

  std::map<std::string, int> Human::attributes() const {
    std::map<std::string, int> attributes = m_attributes;
    for(const Equipment * eq : m_equipments) {
      for(auto e : eq->effects()) {
        attributes[e.first] += e.second;
      }
    }
    return attributes;
  }

	bool Human::drop(Keepable * item) {
		if(location()->drop(this, item)) {
			m_inventory.erase(item);
			m_inventory_weight -= item->weight();
			m_used_inventory_volume -= item->volume();
			return true;
		} else {
			return false;
		}
	}

	bool Human::equip(Equipment * item) {
		Human::slot_t slot = Equipment::default_slot(item->type());
		return equip(item, slot);
	}

	bool Human::equip(Equipment * item, Human::slot_t slot) {

		if(m_inventory.count(dynamic_cast<Keepable*>(item)) == 0) {
			Game::out(location()) << name() << " can't equip " << item->name() << " since it's not in inventory." << std::endl;
			return false;
		}

		if(slot == LEFT_HAND && m_equipments[RIGHT_HAND]->type() == Equipment::TWO_HAND) {
			Game::out(location()) << name() << " can't equip a item in the left hand while a two hand item is equipped in the right." << std::endl;
			return false;
		}

		if(slot != Equipment::default_slot(item->type()) && ! (slot == LEFT_HAND && item->type() == Equipment::ONE_HAND)) {
			Game::out(location()) << name() << " can't equip " << item->name() << " in that slot." << std::endl;
			return false;
		}

		if(m_equipments[slot] != nullptr) {
			store(m_equipments[slot]);
			m_equipments[slot] = nullptr;
		}

		m_equipments[slot] = item;
		m_inventory.erase(dynamic_cast<Keepable*>(item));

		return true;
	}

	bool Human::unequip(Human::slot_t slot) {
		if(m_equipments[slot] != nullptr) {
			store(m_equipments[slot]);
			m_equipments[slot] = nullptr;
			return true;
		} else {
			return false;
		}
	}

	Human::Human(const std::string &name, const std::string &description, faction_t faction, std::map<std::string, int> attributes, Area * location)
		: Character(name, description, faction, attributes, location) {

	}

	void Human::store(Keepable * item) {
		bool store = true;
		if(backpack_volume() - m_used_inventory_volume < item->volume()) {
			Game::out(location()) << name() << " can't store " << item->name() << ", not enought space in backpack." << std::endl;
			store = false;
		} else if(carrying_capacity() - m_inventory_weight < item->weight()) {
			Game::out(location()) << name() << " can't store " << item->name() << ", carrying to much weight." << std::endl;
			store = false;
		}

		if(store) {
			m_inventory_weight += item->weight();
			m_used_inventory_volume += item->volume();
			m_inventory.insert(item);
		} else {
			location()->drop(this, item, true);
		}

	}

	int Human::used_backpack_volume() const {
		return m_used_inventory_volume;
	}

	int Human::used_carrying_capacity() const {
		return m_inventory_weight;;
	}

	Human::~Human() {
		std::for_each(m_inventory.begin(), m_inventory.end(), [](Keepable * i) { delete i; });
	}

	const std::set<Keepable*> &Human::inventory() const {
		return m_inventory;
	}

  void Human::action() {
    Character::action();

    if(m_equipments[RIGHT_HAND] != nullptr) {
      m_remaining_actions[RIGHT_HAND] = m_equipments[RIGHT_HAND]->effect("weapon_actions");
      if(m_equipments[LEFT_HAND] != nullptr) {
        m_remaining_actions[LEFT_HAND] = m_equipments[LEFT_HAND]->effect("weapon_actions");
      } else if( m_equipments[RIGHT_HAND]->type() == Equipment::TWO_HAND) {
        m_remaining_actions[LEFT_HAND] = 0;
      } else {
        m_remaining_actions[LEFT_HAND] = 1;
      }
    } else {
      m_remaining_actions[RIGHT_HAND] = 1;
      if(m_equipments[LEFT_HAND] != nullptr) {
        m_remaining_actions[LEFT_HAND] = m_equipments[LEFT_HAND]->effect("weapon_actions");
      } else {
        m_remaining_actions[LEFT_HAND] = 1;
      }
    }
  }

  void Human::attack(Character * character, int points) {
    if(m_remaining_actions[RIGHT_HAND] > 0) {
      attack(character, points, RIGHT_HAND);
    } else {
      attack(character, points, LEFT_HAND);
    }
  }

  void Human::attack(Character * character, int points, Human::slot_t slot) {
    if(slot > LEFT_HAND) {
      Logging::fatal("Invalid slot specified for attack, only LEFT or RIGHT hand allowed\n");
    }

    if(use_action(slot)) {

    }
    m_state = IN_FIGHT;
    m_in_fight = character;
    /* TODO */
    if(try_do_action(points)) {
      //int damage = T10 + dameg + öp etc
      //Game::out(location()) << name() << " attack " << character->name() << " "
    } else {
      Game::out(location()) << name() << " misses " << character->name() << "." << std::endl;
    }
  }

  void Human::incoming_attack(Character * character, int damage) {
    
  }

}
