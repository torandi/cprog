#include "health_potion.hpp"
#include "character.hpp"

namespace game {
  HealthPotion::HealthPotion(int hp)
    : Item("health potion", "restores some health")
    , m_hp(hp) {
  }
  bool HealthPotion::aquire(Character * character) const {
    character->regain_life(m_hp);
    return false;
  }

  HealthPotion * HealthPotion::from_config(const ConfigNode * node) {
    return new HealthPotion((*node)["/hp"].parse_int());
  }
};
