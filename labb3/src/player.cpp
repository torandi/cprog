#include "player.hpp"
#include "keepable.hpp"
#include "game.hpp"
#include "area.hpp"
#include "input.hpp"

namespace game {

  Player::Player(std::map<std::string, int> attributes, Area * location)
    : Human("You", "", Character::CIVILIAN, attributes, location) {

  }

  void Player::action() {
		char buffer[16];
		while(m_action_points > 0 && Game::singleton->run()) {
			sprintf(buffer, "[AP: %d] >> ", m_action_points);
			Input::read(Input::DEFAULT, buffer);
		}

		if(Game::singleton->run()) std::cout << "Your turn ends." << std::endl;
  }

  void Player::incoming_attack(Character * character, int damage) {

  }

	void Player::next_turn() {
		m_action_points = 0;
	}

	void Player::store(Keepable * item) {
		bool store = true;
		if(backpack_volume() - m_used_inventory_volume < item->volume()) {
			Game::out(location()) << name() << " can't store " << item->name() << ", not enought space in backpack." << std::endl;
			store = false;
		} else if(carrying_capacity() - m_inventory_weight < item->weight()) {
			Game::out(location()) << name() << " can't store " << item->name() << ", carrying to much weight." << std::endl;
			store = false;
		}

		if(store) {
			m_inventory_weight += item->weight();
			m_used_inventory_volume += item->volume();
			m_inventory.insert(item);
		} else {
			location()->drop(this, item, true);
		}

	}

	void Player::attack(Character * character, int points) {
		if(m_remaining_actions[RIGHT_HAND] > 0) {
			attack(character, points, RIGHT_HAND);
		} else if(m_remaining_actions[LEFT_HAND] > 0) {
			attack(character, points, LEFT_HAND);
		} else {
			std::cout << "You don't have any weapon actions left this turn." << std::endl;
		}
	}

	void Player::attack(Character * character, int points, slot_t weapon_hand) {
		Human::attack(character, points, weapon_hand);
	}

	std::string Player::verb(const std::string &verb) const {
		return verb;
	}

	std::string Player::genitive() const {
		return "Your";
	}

	void Player::die() {
		std::cout << "You are dead. Game over." << std::endl;
		Game::singleton->stop();
	}

}
