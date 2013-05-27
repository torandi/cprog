#include "area.hpp"
#include "character.hpp"
#include "config.hpp"
#include "item.hpp"

#include <algorithm>

namespace game {

	Area::Area(const std::string &name, const std::string &description, int movement_cost)
		: m_name(name)
		, m_description(description)
		, m_movement_cost(movement_cost) {

	}

	Area::~Area() { }

	const std::vector<std::string> &Area::directions() const {
		return m_directions;
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
		return true;
	}

	bool Area::leave(Character * character) {
		m_characters.erase(character);
		return true;
	}

	void Area::stay(Character * character) { }

	void Area::set_exits(const std::map<std::string, Area*> &exits) {
		m_exits = exits;
		m_directions.clear();
		std::for_each(exits.begin(), exits.end(), [&](const std::pair<std::string, Area*> it) { m_directions.push_back(it.first); });
	}

	void Area::pick_up(Character * character, Item * item) {
		if(item->pick_up(character)) {
			m_items.erase(item);
		}
	}

	Area * Area::from_config(const ConfigNode * _node) {
		const ConfigNode &node = *_node;
		std::string name = node["/name"].parse_string();
		std::string description = node["/description"].parse_string();
		int movement_cost = node["/movement_cost"].parse_int();
		return new Area(name, description, movement_cost);
	}
}
