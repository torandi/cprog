#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "character.hpp"
#include "game.hpp"
#include "area.hpp"
#include "item.hpp"
#include "keepable.hpp"
#include "human.hpp"
#include "monster.hpp"
#include "logging.hpp"

namespace game {
	bool faction_standings[Character::NUM_FACTIONS][Character::NUM_FACTIONS] = {
										/*  C  M  B  U   */
		/* CIVILIAN: */		{ 0, 0, 0, 0, }, /* Civilians don't attack anyone */
		/* MONSTER: */		{ 1, 0, 1, 1, },
		/* BANDIT: */			{ 1, 1, 0, 1, },
		/* UNALIGNED: */	{ 1, 1, 1, 1, },
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

	int Character::attribute(const std::string &attr) const {
		auto it = m_attributes.find(attr);
		if(it != m_attributes.end()) {
			return it->second;
		} else {
			return 0;
		}
	}

	void Character::action() {
		m_action_points = attribute("action_points");
		m_remaining_actions[0] = m_base_actions[0];
		m_remaining_actions[1] = m_base_actions[1];

		int life_regen = attribute("life_regen");
		if(life_regen > 0) regain_life(life_regen);
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

	void Character::regain_life(int life) {
		int l = std::min(life, max_life() - m_life);
		Game::out(location()) << name() << " regains " << l << " hp." << std::endl;
		m_life += l;
	}

	Character::faction_t Character::faction() const {
		return m_faction;
	}

	std::string Character::name() const {
		return m_name;
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

	void Character::reduce_armor(int amount) {
		int protection_prev = armor_protection();
		m_attributes["armor"] = std::max(0, m_attributes["armor"] - amount);
		int diff = protection_prev - armor_protection();
		if(diff > 0) Game::out(location()) << name() << " loses " << diff << " points of armor protection." << std::endl;
	}

	void Character::hurt(int damage) {
		damage = std::max(0, damage - armor_protection());
		if(damage == 0) return;

		reduce_armor(damage);
		m_life -= damage;
		if(m_life <= 0) {
			m_life = 0;
			die();
		}
	}

	void Character::parse_inventory(Character * character, const ConfigNode * node) {
		if(node == nullptr) return;
		for(const ConfigNode * i : node->list()) {
			Item * item = Item::from_node(i);
			Keepable * keep = dynamic_cast<Keepable*>(item);

			if(keep == nullptr) delete item;
			else character->store(keep);
		}
	}

	Character * Character::from_node(const ConfigNode * node, Area * location) {
		if(node->type == ConfigNode::NODE_MAPPING) {
			return from_config(node, location);
		} else if(node->type == ConfigNode::NODE_SCALAR) {
			std::string name = node->parse_string();
			Config cfg = Config::from_filename("game/npcs.yaml");
			return from_config(&cfg[name], location);
		} else {
			Logging::fatal("NPC node must be mapping or a string naming the npc id\n");
		}
	}

	std::map<std::string, std::function<Character*(const ConfigNode*, Area*)> > Character::tag_map = {
		{"!human", &Human::from_config },
		{"!monster", &Monster::from_config },
	};

}
