#include "monster.hpp"
#include "area.hpp"
#include "item.hpp"
#include "keepable.hpp"

namespace game {

	Monster::Monster(const std::string &name, const std::string &description, std::map<std::string, int> attributes, Area * location)
		: Character(name, description, Character::MONSTERS, attributes, location) {
	}

	Monster * Monster::from_config(const ConfigNode * node, Area * location) {
		std::string name = (*node)["/name"].parse_string();
		std::string description = (*node)["/description"].parse_string();

		std::map<std::string, int> attributes = Character::parse_attributes(&(*node)["/attributes"]);

		Monster * monster = new Monster(name, description, attributes, location);
		Character::parse_inventory(monster, node->find("/inventory", false));
		return monster;
	}

  void Monster::action() {
    init_round();
  }

	void Monster::store(Keepable * item) {
		m_loot.push_back(item);
	}

	void Monster::attack(Character * character, int points) {

	}

	void Monster::reduce_armor(int amount) {
		return; /* Monster's armor don't decay (natural armor) */
	}

	void Monster::die() {
    Character::die();
		Game::out(location()) << name() << " dies in a pool of blood." << std::endl;
		for(Keepable * item : m_loot) {
			location()->drop(this, item, true);
		}
	}

}
