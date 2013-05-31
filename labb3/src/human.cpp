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
	std::string Human::slot_names[Human::NUM_SLOTS] = {
		"right hand",
		"left hand",
		"armor slot",
		"ring slot",
		"backpack slot"
	};

	int Human::backpack_volume() const {
		return attribute("backpack_volume");
	}

	int Human::carrying_capacity() const {
		return 10 * ( 1 + attribute("strength") );
	}

	int Human::attribute(const std::string &attr) const {
		int val = Character::attribute(attr);
		for(const Equipment * e : m_equipments) {
			if(e != nullptr) val += e->effect(attr);
		}
		return val;
	}

  std::map<std::string, int> Human::attributes() const {
    std::map<std::string, int> attributes = m_attributes;
    for(const Equipment * eq : m_equipments) {
			if(eq != nullptr) {
				for(auto e : eq->effects()) {
					attributes[e.first] += e.second;
				}
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
		if(m_inventory.count(item) == 0) {
			if(carrying_capacity() - m_inventory_weight < item->weight()) {
				Game::out(location()) << name() << " can't store " << item->name() << ", carrying to much weight." << std::endl;
				return false;
			} else {
				m_inventory_weight += item->weight();
			}
		}

		if(slot == LEFT_HAND && m_equipments[RIGHT_HAND] != nullptr && m_equipments[RIGHT_HAND]->type() == Equipment::TWO_HAND) {
			Game::out(location()) << name() << " can't equip a item in the left hand while a two hand item is equipped in the right." << std::endl;
			return false;
		}

		if(slot != Equipment::default_slot(item->type()) && ! (slot == LEFT_HAND && item->type() == Equipment::ONE_HAND)) {
			Game::out(location()) << name() << " can't equip " << item->name() << " in " << slot_names[item->type()] << "." << std::endl;
			return false;
		}

		if(m_equipments[slot] != nullptr) {
			store(m_equipments[slot]);
			m_equipments[slot] = nullptr;
		}

		m_equipments[slot] = item;
		m_inventory.erase(item);

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
		m_inventory_weight += item->weight();
		m_used_inventory_volume += item->volume();
		m_inventory.insert(item);
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

	Equipment * Human::equipment(Human::slot_t slot) const {
		return m_equipments[slot];
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

	Human * Human::from_config(const ConfigNode * node, Area * location) {
		std::string name = (*node)["/name"].parse_string();
		std::string description = (*node)["/description"].parse_string();

		faction_t faction;
		std::string str_faction = (*node)["/faction"].parse_string();
		if(str_faction.find("bandit") != str_faction.npos) {
			faction = BANDITS;
		} else if(str_faction.find("civ") != str_faction.npos) {
			faction = CIVILIAN;
		} else if(str_faction.find("unal") != str_faction.npos) {
			faction = UNALIGNED;
		} else {
			Logging::fatal("Unknown faction %s for human %s\n", str_faction.c_str(), name.c_str());
		}
		std::map<std::string, int> attributes = Character::parse_attributes(&(*node)["/attributes"]);
		Human * human = new Human(name, description, faction, attributes, location);

		parse_equipment(human, node->find("/equipment", false));
		Character::parse_inventory(human, node->find("/inventory", false));

		const ConfigNode * dialog = node->find("/dialog", false);
		if(dialog) {
			for(const ConfigNode * d : dialog->list()) {
				human->m_dialog.push_back(d->parse_string());
			}
		}

		return human;

	}

	void Human::parse_equipment(Human * human, const ConfigNode * node) {
		if(node == nullptr) return;
		for(const ConfigNode * i : node->list()) {
			Item * item = Item::from_node(i);
			Equipment * eq = dynamic_cast<Equipment*>(item);

			if(eq != nullptr) {
				human->m_inventory.insert(eq);
				slot_t slot = Equipment::default_slot(eq->type());
				if(i->type == ConfigNode::NODE_MAPPING) {
					const ConfigNode * hand = i->find("/hand", false);
					if(hand != nullptr && hand->parse_string() == "left") slot = LEFT_HAND;
				}
				if(!human->equip(eq, slot)) delete item;
			} else {
				delete item;
			}
		}
	}

	void Human::die() {
		Game::out(location()) << name() << " gives up a cough of blood and drops dead." << std::endl;
		for(int i=0; i<NUM_SLOTS; ++i) {
			unequip(static_cast<slot_t>(i));
		}
		for(Keepable * item : m_inventory) {
			location()->drop(this, item, true);
		}
	}

	bool Human::talk_to(Human * human) {
		if(m_dialog.size() == 0) return false;

		Game::out(location()) << m_dialog[m_next_dialog] << std::endl;
		m_next_dialog = (m_next_dialog + 1) % static_cast<int>(m_dialog.size());
		return true;
	}

}
