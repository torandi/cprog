#ifndef ANCIENT_STONE_HPP
#define ANCIENT_STONE_HPP

#include "item.hpp"
#include "config.hpp"

namespace game {
  class AncientStone : public Item {
    public:
      static AncientStone * from_config(const ConfigNode * node);
			virtual std::string description() const;

			/* Returns true if the item should now be deleted */
			virtual bool use(Character * character) const;
      virtual bool pick_up(Character * character) const { return false; };
      virtual bool aquire(Character * character) const { return true; };

      virtual ~AncientStone() {};
    protected:
      AncientStone(const std::string &name, const std::string &description, const std::string &spawn_enemy);

      enum {
        PURPLE=0,
        RED,
        BLUE
      };

      std::string m_spawn_enemy;
      mutable bool m_spawned_enemy;
      mutable bool m_placed_stones[3] = {false, };
      mutable int m_stone_count;

      static const std::string m_stone_ids[3];
  };
};

#endif
