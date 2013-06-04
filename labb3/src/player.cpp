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
		m_next_turn = false;
		while(!m_next_turn && m_action_points > 0 && Game::singleton->run()) {
			sprintf(buffer, "[AP: %d] >> ", m_action_points);
			Input::read(Input::DEFAULT, buffer);
		}

		if(Game::singleton->run()) std::cout << "Your turn ends." << std::endl;
  }

  void Player::incoming_attack(Character * character, int damage, bool critical_hit) {
		m_state = IN_FIGHT;
		m_in_fight = character;

		if(m_block_decision.first > -1) {
			if(block(damage, m_block_decision.second /* points */, static_cast<Human::slot_t>(m_block_decision.first) /* weapon hand */)) return;
		} else if(!critical_hit && passive_protection(character, damage)) {
      return;
    }

		hurt(damage);
	}

	void Player::pre_damage(Character * character) {
		char buffer[32];
		sprintf(buffer, "[AP: %d] block? >> ", m_action_points);
		std::cout << "You are being attacked by " << character->name() << "." << std::endl;

		bool valid_action = false;
		do {
			m_block_decision.first = -2;
			while(Game::singleton->run() && m_block_decision.first == -2) Input::read(Input::DEFEND, buffer, (void*)&m_block_decision);
			if(m_block_decision.first == -1) valid_action = true;
			else if(m_remaining_actions[m_block_decision.first] == 0) {
				std::cout << "You can't block with " << slot_names[m_block_decision.first] << ", you don't have any actions left." << std::endl;
			} else if(m_action_points < m_block_decision.second) {
				std::cout << "You only have "  << m_action_points << " left, only blocking for them." << std::endl;
				m_block_decision.second = m_action_points;
				valid_action = true;
			} else {
				valid_action = true;
			}
		} while(!valid_action && Game::singleton->run());
  }

	void Player::next_turn() {
		m_next_turn = true;
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

	bool Player::is_equipped(const Equipment * equipment) const {
		for(Equipment * eq : m_equipments) {
			if(eq == equipment) return true;
		}
		return false;
	}

}
