#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "character.hpp"
#include "config.hpp"

namespace game {
	class Monster : public Character {
		public:
      virtual void action();

			virtual void attack(Character * character, int points);

			static Monster * from_config(const ConfigNode * node, Area * location);

      virtual ~Monster();
			virtual void store(Keepable * item);
			virtual void delete_item(Keepable  * item);
		protected:
			Monster(const std::string &name, const std::string &description, const std::string &weapon_name, std::map<std::string, int> attributes, Area * location);

			virtual void die();
			virtual void reduce_armor(int amount);

			std::vector<Keepable*> m_loot; /* Stuff that drop when the monster die */
      const std::string m_weapon_name;
	};
};

#endif
