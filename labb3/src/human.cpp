#include "human.hpp"
#include "keepable.hpp"
#include "equipment.hpp"
#include "area.hpp"
#include "game.hpp"

#include <algorithm>

namespace game {

	int Human::backpack_volume() const {
		return 5 + attribute("backpack_volume");
	}

	int Human::carrying_capacity() const {
		return 10 * ( 1 + attribute("strength") );
	}

	int Human::attribute(const std::string &attr) const {
		int val = Character::attribute(attr);
		for(const Equipment * e : equipments) {
			val += e->effect(attr);
		}
		return val;
	}

	bool Human::drop(Keepable * item) {
		if(location()->drop(this, item)) {
			m_inventory.erase(item);
			m_inventory_weight -= item->weight();
			m_used_inventory_volume -= item->volume();
			return true;
		} else {
			return false;
		}
	}

	bool Human::equip(Equipment * item) {
		Human::slot_t slot = Equipment::default_slot(item->type());
		return equip(item, slot);
	}

	bool Human::equip(Equipment * item, Human::slot_t slot) {

		if(m_inventory.count(dynamic_cast<Keepable*>(item)) == 0) {
			Game::out(location()) << name() << " can't equip " << item->name() << " since it's not in inventory." << std::endl;
			return false;
		}

		if(slot == LEFT_HAND && equipments[RIGHT_HAND]->type() == Equipment::TWO_HAND) {
			Game::out(location()) << name() << " can't equip a item in the left hand while a two hand item is equipped in the right." << std::endl;
			return false;
		}

		if(slot != Equipment::default_slot(item->type()) && ! (slot == LEFT_HAND && item->type() == Equipment::ONE_HAND)) {
			Game::out(location()) << name() << " can't equip " << item->name() << " in that slot." << std::endl;
			return false;
		}

		if(equipments[slot] != nullptr) {
			store(equipments[slot]);
			equipments[slot] = nullptr;
		}

		equipments[slot] = item;
		m_inventory.erase(dynamic_cast<Keepable*>(item));

		return true;
	}

	bool Human::unequip(Human::slot_t slot) {
		if(equipments[slot] != nullptr) {
			store(equipments[slot]);
			equipments[slot] = nullptr;
			return true;
		} else {
			return false;
		}
	}

	Human::Human(const std::string &name, const std::string &description, faction_t faction, std::map<std::string, int> attributes, Area * location)
		: Character(name, description, faction, attributes, location) {

	}

	void Human::store(Keepable * item) {
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

	int Human::used_backpack_volume() const {
		return m_used_inventory_volume;
	}

	int Human::used_carrying_capacity() const {
		return m_inventory_weight;;
	}

	Human::~Human() {
		std::for_each(m_inventory.begin(), m_inventory.end(), [](Keepable * i) { delete i; });
	}

	const std::set<Keepable*> &Human::inventory() const {
		return m_inventory;
	}

}
