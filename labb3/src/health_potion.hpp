#ifndef HEALTH_POTION_HPP
#define HEALTH_POTION_HPP

#include "item.hpp"
#include "config.hpp"

namespace game {
  class HealthPotion : public Item {
    public:
      virtual bool pick_up(Character * character) const;
      virtual void aquire(Character * character) const;

      static HealthPotion * from_config(const ConfigNode * node);

      virtual ~HealthPotion() {};
    protected:
      HealthPotion(int hp);
      int m_hp;
  };
};

#endif
