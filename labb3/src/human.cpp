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
#include <sstream>

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
			Game::out(location()) << name() << " " << verb("drop") << " " << item->name() << std::endl;
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
				Game::out(location()) << name() << " can't store " << item->name() << ", carrying too much weight." << std::endl;
				return false;
			} else {
				m_inventory_weight += item->weight();
			}
		} else {
			m_used_inventory_volume -= item->volume();
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
	}

  void Human::init_round() {
    Character::init_round();

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
		} else if(m_remaining_actions[LEFT_HAND] > 0) {
      attack(character, points, LEFT_HAND);
    }
  }

  void Human::attack(Character * character, int points, Human::slot_t slot) {
    if(slot > LEFT_HAND) {
      Logging::fatal("Invalid slot specified for attack, only LEFT or RIGHT hand allowed\n");
    }

		if(m_equipments[slot] != nullptr && m_equipments[slot]->type() > Equipment::TWO_HAND) {
			Game::out(location()) << name() << " " << verb("can't") << " attack with " << m_equipments[slot]->raw_name() << "." << std::endl;
			return;
		}

		if(m_action_points < points) {
			throw "More action points needed.";
		}


    if(!use_action(slot)) {
			Game::out(location()) << name() << " " << verb("don't") << " have any actions left in " << slot_names[slot] << "." << std::endl;
			return;
		}

		if(m_equipments[slot] != nullptr) {
			roll_attack(Game::T10, points, character,
					m_equipments[slot]->effect("damage_overpower", -1),
					m_equipments[slot]->effect("damage_extra"),
					" with " + m_equipments[slot]->raw_name()
				);
		} else {
			roll_attack(Game::T5, points, character, -1, 0, " with " + slot_names[slot]);
		}

  }

  void Human::incoming_attack(Character * character, int damage) {
		bool blocked = false;
		int points = std::min(15, m_action_points);

		if(points > 0) {
			if(m_equipments[LEFT_HAND] != nullptr && m_remaining_actions[LEFT_HAND] > 0) {
				blocked = block(damage, points, LEFT_HAND);
			} else if(m_remaining_actions[RIGHT_HAND] > 0) {
				blocked = block(damage, points, RIGHT_HAND);
			} else if( m_remaining_actions[LEFT_HAND] > 0) {
				blocked = block(damage, points, LEFT_HAND);
			}
		}
		if(!blocked) hurt(damage);
  }

	bool Human::block(int damage, int points, slot_t slot) {
		if(m_action_points < points) {
			throw "More action points needed.";
		}

		if(!use_action(slot)) {
			Game::out(location()) << name() << " " << verb("don't") << " have any actions left in " << slot_names[slot] << "." << std::endl;
			return false;
		}

		std::string weapon_name = slot_names[slot];
		Equipment * weapon = m_equipments[slot];
		if(weapon != nullptr) weapon_name = weapon->raw_name();

		Game::try_result_t roll = try_do_action(points);
		switch(roll) {
			case Game::PERFECT:
				Game::out(location()) << name()<< " " << verb("block") << " perfectly with " << weapon_name << ". All damage absorbed." << std::endl;
				return true;
			case Game::SUCCESS:
				if(weapon != nullptr) {
					int dmg = damage - std::max(1, weapon->effect("durability") / 10);
					if(dmg > 0) {
						Game::out(location()) << name() << " " << verb("block") << " with " << weapon_name << ". " << weapon_name << " take " << dmg << " damage." << std::endl;
						weapon->reduce("durability", dmg);
						if(weapon->effect("durability") <= 0) {
							Game::out(location()) << genitive() << " " << weapon_name << " have taken too much damage and falls apart." << std::endl;
							m_inventory_weight -= weapon->weight();
							m_equipments[slot] = nullptr;
							delete weapon;
						}
					} else {
						Game::out(location()) << name() << " " << verb("block") << " with " << weapon_name << ". " << weapon_name << " absorbs all damage." << std::endl;
					}
				} else {
					Game::out(location()) << name() << " " << verb("block") << " with " << weapon_name << ", reducing the damage with 1." << std::endl;
					hurt(damage - 1);
				}
				return true;
			case Game::FAIL:
				Game::out(location()) << name() << " " << verb("fail") << " to block the attack." << std::endl;
				return false;
			case Game::FATAL:
				Game::out(location()) << name() << " " << verb("fail") << " misserably to block and have -5 on next attack or block." << std::endl;
				m_tmp_action_mod = -5;
				return false;
		}
		return false;
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

	void Human::reduce_armor(int amount) {
		if(m_equipments[ARMOR] != nullptr) {
			int protection_prev = armor_protection();
			m_equipments[ARMOR]->reduce("armor", amount);
			int diff = protection_prev - armor_protection();
			if(diff > 0) Game::out(location()) << genitive() << " armor lost " << diff << " point" <<
				(diff > 1 ? "s" : "")
				<< " of armor protection." << std::endl;
		}
	}

	void Human::die() {
		Game::out(location()) << name() << " gives up a cough of blood and drops dead." << std::endl;
		for(int i=0; i<NUM_SLOTS; ++i) {
			unequip(static_cast<slot_t>(i));
		}
		std::for_each(m_inventory.begin(), m_inventory.end(), [&](Keepable * k) {
			drop(k);
		});
	}

	bool Human::talk_to(Human * human) {
		if(m_dialog.size() == 0) return false;

		Game::out(location()) << m_dialog[m_next_dialog] << std::endl;
		m_next_dialog = (m_next_dialog + 1) % static_cast<int>(m_dialog.size());
		return true;
	}

	std::string Human::description() const {
		std::stringstream str;
		str << Character::description() << std::endl
			<< "Equipment: " <<std::endl;
		for(const Equipment * eq : m_equipments) {
			if(eq != nullptr) str << eq->raw_name() << std::endl;
		}
		return str.str();
	}

}
