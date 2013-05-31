#include "health_potion.hpp"
#include "character.hpp"
#include <sstream>

namespace game {
  HealthPotion::HealthPotion(int hp)
    : Keepable("Health potion", "restores some health", 1, 0)
    , m_hp(hp) {
  }

	bool HealthPotion::use(Character * character) const {
		Game::out(character->location()) << character->name() << " " << character->verb("use") << " " << name() << std::endl;
		character->regain_life(m_hp);
		return true;
	}

  HealthPotion * HealthPotion::from_config(const ConfigNode * node) {
    return new HealthPotion((*node)["/hp"].parse_int());
  }

	std::string HealthPotion::name() const {
		std::stringstream str;
		str << Keepable::name() << " [HP: " << m_hp << "]";
		return str.str();
	}
};
