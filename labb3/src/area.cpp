#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "area.hpp"
#include "character.hpp"
#include "config.hpp"
#include "item.hpp"
#include "keepable.hpp"
#include "logging.hpp"
#include "container.hpp"
#include "player.hpp"
#include "darkwood.hpp"

#include <algorithm>

namespace game {

	Area::Area(const std::string &name, const std::string &description, int movement_cost)
		: m_name(name)
		, m_description(description)
		, m_movement_cost(movement_cost) {

	}

	Area::~Area() {
		std::for_each(m_items.begin(), m_items.end(), [](Item* i) { delete i; });
	}

	int Area::movement_cost() const {
		return m_movement_cost;
	}

	Area* Area::neighbor(const std::string &direction) const {
		const auto item = m_exits.find(direction);
		if(item == m_exits.end()) return nullptr;
		return item->second;
	}

	const std::string &Area::name() const {
		return m_name;
	}

	const std::string &Area::description() const {
		return m_description;
	}

	bool Area::enter(Character * character) {
		m_characters.insert(character);
		if(character != Game::player()) Game::out(this) << character->name() << " enters " << name() << std::endl;
		return true;
	}

  bool Area::contains_character(Character * character) const {
    return m_characters.count(character) == 1;
  }

	bool Area::leave(Character * character) {
		m_characters.erase(character);
		if(character != Game::player()) Game::out(this) << character->name() << " leaves " << name() << std::endl;
		return true;
	}

	void Area::remove_character(Character * character) {
		m_characters.erase(character);
	}

	void Area::stay(Character * character) { }

	void Area::set_exits(const std::map<std::string, Area*> &exits) {
		m_exits = exits;
		m_directions.clear();
		std::for_each(exits.begin(), exits.end(), [&](const std::pair<std::string, Area*> it) { m_directions.push_back(it.first); });
	}

	bool Area::pick_up(Character * character, Item * item) {
		if(item->pick_up(character)) {
			m_items.erase(item);
			return true;
		} else {
			return false;
		}
	}

	bool Area::drop(Character * character, Item * item, bool force) {
		if(!force) {
			Keepable * k = dynamic_cast<Keepable*>(item);
			if(!k->drop(character)) return false;
		}
		m_items.insert(item);
		return true;
	}

  const std::set<Character*> &Area::characters() const {
    return m_characters;
  }

	const std::set<Item*> &Area::items() const {
		return m_items;
	}

  std::set<Item*> Area::all_items() const {
		std::set<Item*> items(m_items);
		for(Item * i : m_items) {
			Container * c = dynamic_cast<Container*>(i);
			if(c != nullptr && c->is_open()) {
				for(Keepable * k : c->content()) {
						items.insert(k);
				}
			}
		}
		return items;
  }

	const std::map<std::string, Area*> &Area::exits() const {
		return m_exits;
	}

	Area * Area::self_from_config(const ConfigNode * _node) {
		const ConfigNode &node = *_node;
		std::string name = node["/name"].parse_string();
		std::string description = node["/description"].parse_string();
		int movement_cost = node["/movement_cost"].parse_int();
		return new Area(name, description, movement_cost);
	}

  Area * Area::from_config(const ConfigNode * node) {
    auto it = tag_map.find(node->tag());
    if(it == tag_map.end()) Logging::fatal("Unknown area tag %s\n", node->tag().c_str());
    return (it->second)(node);
  }

  std::map<std::string, std::function<Area*(const ConfigNode*)> > Area::tag_map = {
    {"!area", &Area::self_from_config },
		{"!darkwood", &Darkwood::self_from_config },
  };
}
