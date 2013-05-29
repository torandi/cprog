#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "human.hpp"

namespace game {
  class Area;
  class Player : public Human {
    public:
      virtual ~Player() {};
      Player(std::map<std::string, int> attributes, Area * location);

      virtual void attack(Character * character);
      virtual void incoming_attack(Character * character, int damage);

      virtual void action();

    private:

  }

};

#endif
