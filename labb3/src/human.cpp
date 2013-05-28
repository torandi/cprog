#include "human.hpp"

int Human::backpack_volume() const {
	return 5 + attribute("backpack_volume");
}

int Human::attribute(const std::string &attr) const {
	int base = Character::attribute(attr);
	for(const Equipment * e : equipments) {
		base += e->effect(attr);
	}
}
