#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "character.hpp"
#include "config.hpp"

namespace game {
	class Monster : public Character {
		public:
			virtual void attack(Character * character, int points);
			virtual void incoming_attack(Character * character, int damage);

			static Monster * from_config(const ConfigNode * node, Area * location);
		protected:
			Monster(const std::string &name, const std::string &description, std::map<std::string, int> attributes, Area * location);
			virtual void store(Keepable * item);

			virtual void die();
			virtual void reduce_armor(int amount);

			std::vector<Keepable*> m_loot; /* Stuff that drop when the monster die */
	};
};

#endif