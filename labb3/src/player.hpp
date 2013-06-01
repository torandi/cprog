#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "human.hpp"

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
  };

};

#endif
