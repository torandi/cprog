#include "human.hpp"
#include "keepable.hpp"
#include "equipment.hpp"

namespace game {

	int Human::backpack_volume() const {
		return 5 + attribute("backpack_volume");
	}

	int Human::carrying_capacity() const {
		return 10 * ( 1 + attribute("strength") );
	}

	int Human::attribute(const std::string &attr) const {
		int base = Character::attribute(attr);
		for(const Equipment * e : equipments) {
			base += e->effect(attr);
		}
	}

	bool Human::drop(Keepable * item) {

	}

	bool equip(Keepable * item) {
		Human::slot_t slot = Equipment::default_slot(item->type());
		return equip(item, slot);
	}

	bool Human::equip(Equipment * item, Human::slot_t slot) {

		if(m_inventory->count(dynamic_cast<Keepable*>(item)) == 0) {
			Game::out(location()) << name() << " can't equip " << item->name() << " since it's not in the inventory." << std::endl;
			return false;
		}

		if(slot == LEFT_HAND && equipments[RIGHT_HAND]->type() == Equipment::TWO_HAND) {
			Game::out(location()) << name() << " can't equip a item in the left hand while a two hand item is equipped in the right." << std::endl;
			return false;
		}

		if(slot != Equipment::default_slot(item->type()) && ! (slot == LEFT_HAND && item->type() == ONE_HAND)) {
			Game::out(location()) << name() << " can't equip " << item->name() << " in that slot." << std::endl;
			return false;
		}

		if(equipments[slot] != nullptr) {
			store(equipments[slot]);
			equipments[slot] = nullptr;
		}

		equipments[slot] = item;
		m_inventory->erase(dynamic_cast<Keepable*>(item));

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

}
