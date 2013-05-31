#ifndef HEALTH_POTION_HPP
#define HEALTH_POTION_HPP

#include "keepable.hpp"
#include "config.hpp"

namespace game {
  class HealthPotion : public Keepable {
    public:
      static HealthPotion * from_config(const ConfigNode * node);
			virtual std::string name() const;

			/* Returns true if the item should now be deleted */
			virtual bool use(Character * character) const;

      virtual ~HealthPotion() {};
    protected:
      HealthPotion(int hp);
      int m_hp;
  };
};

#endif
