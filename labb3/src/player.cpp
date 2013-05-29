#include "player.hpp"

namespace game {

  Player::Player(std::map<std::string, int> attributes, Area * location)
    : Human("You", "", Character::CIVILIAN, attributes, location) {

  }

  void Player::action() {

  }

}
