#include "area.hpp"
#include "character.hpp"

namespace game {

	Area::Area(const std::string &name, const std::string &description, int movement_cost)
		: m_name(name)
		, m_description(description)
		, m_movement_cost(movement_cost) {

	}

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
}
