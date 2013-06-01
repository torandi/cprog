#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "human.hpp"

typedef std::pair<int, int> player_block_ret_t;

namespace game {
  class Area;
  class Player : public Human {
    public:
      Player(std::map<std::string, int> attributes, Area * location);
      virtual ~Player() {};

			virtual void attack(Character * character, int points);
			virtual void attack(Character * character, int points, slot_t weapon_hand);
      virtual void incoming_attack(Character * character, int damage);

      virtual void action();
			virtual std::string verb(const std::string &verb) const;
			virtual std::string genitive() const;

			void next_turn();
		protected:
			void store(Keepable * item);
			virtual void die();

			virtual void pre_damage(Character * character);
		private:
			player_block_ret_t m_block_decision;
			bool m_next_turn;
  };

};

#endif
