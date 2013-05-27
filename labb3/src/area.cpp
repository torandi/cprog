#include "area.hpp"
#include "character.hpp"

namespace game {

	Area::Area(const std::string &description)
		: m_description(description) {

	}

	const std::vector<std::string> &Area::directions() const {
		return m_directions;
	}

	Area* Area::neighbor(const std::string &direction) const {
		const auto item = m_exits.find(direction);
		if(item == m_exits.end()) return nullptr;
		return item->second;
	}

	const std::string &Area::description() const {
		return m_description;
	}

	void Area::enter(Character * character) {
		m_characters.insert(character);
	}

	void Area::leave(Character * character) {
		m_characters.erase(character);
	}

	void Area::stay(Character * character) { }
}
