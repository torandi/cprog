#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "character.hpp"
#include "container.hpp"
#include "game.hpp"
#include "area.hpp"
#include "item.hpp"
#include "keepable.hpp"
#include "human.hpp"
#include "monster.hpp"
#include "requiring_character.hpp"
#include "logging.hpp"
#include "color.hpp"

namespace game {
	bool Character::faction_standings[Character::NUM_FACTIONS][Character::NUM_FACTIONS] = {
										/*  C  M  B  U  I*/
		/* CIVILIAN: */		{ 0, 0, 0, 0, 0}, /* Civilians don't attack anyone */
		/* MONSTER: */		{ 1, 0, 1, 1, 0 },
		/* BANDIT: */			{ 1, 1, 0, 1, 0 },
		/* UNALIGNED: */	{ 1, 1, 1, 1, 0 },
		/* INVINCIBLE */ 	{ 0, 0, 0, 0, 0 },
	};

	Character::Character(const std::string &name, const std::string &description, Character::faction_t faction, std::map<std::string, int> attributes, Area * location)
		: m_name(name)
		, m_description(description)
		, m_faction(faction)
		, m_attributes(attributes)
		, m_life(max_life())
		, m_location(location) {
	}

	Character::~Character() {

	}

	int Character::attribute(const std::string &attr, int default_val) const {
		auto it = m_attributes.find(attr);
		if(it != m_attributes.end()) {
			return it->second;
		} else {
			return default_val;
		}
	}

	int Character::extra_damage() const {
		return attribute("strength") / 10;
	}

	void Character::init_round() {
		m_action_points = attribute("action_points");
		m_remaining_actions[0] = m_base_actions[0];
		m_remaining_actions[1] = m_base_actions[1];

		int life_regen = attribute("life_regen");
		if(life_regen > 0) regain_life(life_regen);

		search_for_enemy();
	}

	void Character::search_for_enemy() {
		if(m_in_fight != nullptr && m_in_fight->state() == DEAD) end_fight(m_in_fight);
		if(m_state == IN_FIGHT && location()->contains_character(m_in_fight) == 0) {
			m_enemy_direction = "";
			for(auto exit : location()->exits()) {
				if(exit.second->contains_character(m_in_fight)) {
					m_enemy_direction = exit.first;
				}
			}
			if(m_enemy_direction.empty()) {
				Game::out(location(), m_in_fight->location()) << name() << " " << verb("are") << " no longer in a fight with " << m_in_fight->name() << "." << std::endl;
				m_in_fight->end_fight(this);
				end_fight(m_in_fight);
			}
		}
	}

	bool Character::use_action(int hand) {
		if(m_remaining_actions[hand] > 0) {
			--m_remaining_actions[hand];
			return true;
		} else {
			return false;
		}
	}

	const int Character::life() const {
		return m_life;
	}

	const int Character::max_life() const {
		return attribute("life");
	}

	std::string Character::verb(const std::string &verb) const {
		auto it = m_third_person_verbs.find(verb);
		if(it != m_third_person_verbs.end()) return it->second;
		if(verb[verb.size()-1] == 's') return verb + "es";
		else return verb + "s";
	}

	void Character::regain_life(int life) {
		int l = std::min(life, max_life() - m_life);
		Game::out(location()) << name() << " " << verb("regain") << " " << l << " hp." << std::endl;
		m_life += l;
	}

	Character::faction_t Character::faction() const {
		return m_faction;
	}

	std::string Character::name() const {
		return m_name;
	}

	std::string Character::genitive() const {
		return m_name + "'s";
	}

	std::string Character::description() const {
		return m_description;
	}

	Character::state_t Character::state() const {
		return m_state;
	}

	Area* Character::location() const {
		return m_location;
	}

	bool Character::go(const std::string &direction) {
		Area * new_location = m_location->neighbor(direction);

		if(new_location == nullptr) return false;

		if(attribute("action_points") < new_location->movement_cost()) {
			Game::out(location()) << name() << " " << verb("don't") << " have enough action points to go to " << new_location->name() << std::endl;
			return false;
		}
		if(m_action_points < new_location->movement_cost()) throw "More action points needed.";

		if(m_location->leave(this)) {
			do_action(new_location->movement_cost());
			if(new_location->enter(this)) {
				m_location = new_location;
			} else {
				m_location->enter(this);
			}
			return true;
		}
		return false;
	}

  int Character::initiative() const {
    return m_initiative;
  }

	void Character::roll_initiative() {
		m_initiative = attribute("initiative") + Game::roll_dice(Game::T10, 10);
	}

	int Character::armor_protection() const {
		int a = attribute("armor") / 10;
		if(a == 0 && attribute("armor") > 0) return 1;
		else return a;
	}

	void Character::do_action(int cost) {
		if(m_action_points >= cost) {
			m_action_points -= cost;
		} else {
			throw "More action points needed.";
		}
	}

	Game::try_result_t Character::try_do_action(int cost) {
		do_action(cost);

		Game::try_result_t res = Game::try_action(cost + m_tmp_action_mod + attribute("action_mod"));
		m_tmp_action_mod = 0;

		return res;
	}

	void Character::take(Keepable * item, Container * from) {
		if(from->take(item)) {
			try {
				do_action(5);
			} catch (const char * err) {
				from->put(item);
				throw err;
			}
			Game::out(location()) << name() << " " << verb( "pick") << " up " << item->name() << " from " << from->name() << ". "<< std::endl;
			if(item->aquire(this)) {
				store(dynamic_cast<Keepable*>(item));
			} else {
				delete item;
			}
		}
	}

	void Character::use(Item * item) {
		if(item->use(this)) {
			delete item;
		}
	}

	void Character::pick_up(Item * item) {
		if(m_location->pick_up(this, item)) {
			try {
				do_action(5);
			} catch (const char * err) {
				m_location->drop(this, item, true);
				throw err;
			}
			Game::out(location()) << name() << " " << verb( "pick") << " up " << item->name() << std::endl;
			if(item->aquire(this)) {
				store(dynamic_cast<Keepable*>(item));
			} else {
				delete item;
			}
		}
	}

	std::map<std::string, int> Character::parse_attributes(const ConfigNode * node) {
		std::map<std::string, int> attributes;
		for(auto attr : node->map()) {
			attributes[attr.first] = attr.second->parse_int();
		}
		return attributes;
	}

	Character * Character::from_config(const ConfigNode * node, Area * location) {
		auto it = tag_map.find(node->tag());
		if(it == tag_map.end()) Logging::fatal("Unknown character tag %s\n", node->tag().c_str());
		return (it->second)(node, location);
	}

	void Character::hurt(int damage) {
		if(damage <= 0) return;
		if(m_state == DEAD) return;
		damage = std::max(0, damage - armor_protection());
		if(damage == 0) {
			Game::out(location()) << genitive() << " armor absorbs all damage." << std::endl;
			return;
		} else if(armor_protection() > 0) {
			Game::out(location()) << genitive() << " armor absorbs " << lightcyan <<  armor_protection() << normal << " damage." << std::endl;
		}

		Game::out(location()) << name() << " " << verb("take") << " " << lightred <<  damage << normal << " damage." << std::endl;

		reduce_armor(damage);
		m_life -= damage;
		if(m_life <= 0) {
			m_life = 0;
			m_state = DEAD;
			die();
      Game::singleton->character_dies(this);
			m_in_fight->end_fight(this);
			location()->remove_character(this);
		}
	}

	std::map<std::string, int> Character::attributes() const {
		return m_attributes;
	}

	int Character::remaining_actions(int hand) const {
		return m_remaining_actions[hand];
	}

	void Character::incoming_attack(Character * character, int damage, bool critical_hit) {
		hurt(damage);
	}

	void Character::parse_inventory(Character * character, const ConfigNode * node) {
		if(node == nullptr) return;

    WorldParser::current_prefix_probability = node->find("/prefix_probability");

    const ConfigNode * content = node->find("/random_count");
    if(content != nullptr) {
      Config items_config = Config::from_filename("game/items.yaml");
      int rnd_count = content->parse_int();
      content = node->find("/random_content");
      if(content == nullptr) {
        content = &items_config["/random_items"];
      }
      for(Keepable * item : WorldParser::random_items(content->list(), rnd_count)) {
        character->store(item);
      }
    }

    node = node->find("/items");

    if(node == nullptr) {
      WorldParser::current_prefix_probability = nullptr;
      return;
    }

		for(const ConfigNode * i : node->list()) {
			Item * item = Item::from_node(i);
			Keepable * keep = dynamic_cast<Keepable*>(item);

			if(keep == nullptr) delete item;
			else character->store(keep);
		}

    WorldParser::current_prefix_probability = nullptr;
	}

  Character * Character::from_id(const std::string &id, Area * location) {
    Config cfg = Config::from_filename("game/npcs.yaml");
    return from_config(&cfg[id], location);
  }

	Character * Character::from_node(const ConfigNode * node, Area * location) {
		if(node->type == ConfigNode::NODE_MAPPING) {
			return from_config(node, location);
		} else if(node->type == ConfigNode::NODE_SCALAR) {
			std::string name = node->parse_string();
      return from_id(name, location);
		} else {
			Logging::fatal("NPC node must be mapping or a string naming the npc id\n");
		}
	}

	void Character::start_fight(Character * character) {
		m_state = IN_FIGHT;
		m_in_fight = character;
	}

	void Character::end_fight(Character * character) {
		if(m_in_fight == character && m_state == IN_FIGHT) {
			m_in_fight = nullptr;
			m_state = IDLE;
		}
	}

	void Character::roll_attack(Game::dice_t dice, int points, Character * character, int op, int extra, int num_dices, const std::string &weapon_text) {
		start_fight(character);
		character->start_fight(this);

		Logging::verbose("%s (roll_attack) p: %d, mod: %d, tmpmod: %d\n", name().c_str(), points, attribute("action_mod"), m_tmp_action_mod);
		Game::try_result_t roll = try_do_action(points);

		if(roll < Game::FAIL) {
			int dmg = extra + extra_damage();
      bool critical_hit = false;

      for(int i=0; i<num_dices; ++i) dmg += Game::roll_dice(dice, op);

			std::string extra = "";
			if(roll == Game::PERFECT) {
				dmg += Game::roll_dice(dice, op);
				extra = " Critical hit!";
        critical_hit = true;
			}
			character->pre_damage(this);

			Game::out(location()) << name() << " " << verb("attack") << " "<< character->name() << weapon_text << " for " << lightred << dmg << normal << " damage." << extra << std::endl;

			character->incoming_attack(this, dmg, critical_hit);
		} else if(roll == Game::FATAL) {
			Game::out(location()) << name() << " " << verb("miss") << " " << character->name() << " fataly. " << genitive() << "  attack or block have -5 to succeed." << std::endl;
			m_tmp_action_mod = -5;
		} else {
			Game::out(location()) << name() << " " << verb("miss") << " " << character->name() << "." << std::endl;
		}

	}

	UniqueItem* Character::have_unique(const std::string &id) const {
		return nullptr;
	}

	Character * Character::in_fight_with() const {
		return m_in_fight;
	}

	int Character::remaining_action_points() const {
		return m_action_points;
	}

	int Character::tmp_action_mod() const {
		return m_tmp_action_mod;
	}

	std::map<std::string, std::function<Character*(const ConfigNode*, Area*)> > Character::tag_map = {
		{"!human", &Human::from_config },
		{"!monster", &Monster::from_config },
		{"!requiring", &RequiringCharacter::from_config },
	};

	std::map<std::string, std::string> Character::m_third_person_verbs = {
		{ "don't", "doesn't" },
		{ "do" , "does" },
		{ "have", "has" },
		{ "are", "is" },
	};

}
