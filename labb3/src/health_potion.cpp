#include "health_potion.hpp"

namespace game {
  HealthPotion::HealthPotion(int hp) {
    : m_hp(hp) {
  }
  void HealthPotion::aquire(Character * character) const {
    character->regain_life(m_hp);
  }
};
