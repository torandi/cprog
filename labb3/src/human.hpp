#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "character.hpp"

class Human : public Character {
	public:

		virtual int backpack_size() const;
		//virtual void talk_to(Character * character);
		//drop
		//equip

	protected:
		//Equipment * equipments[NUM_EQUIPMENT_SLOTS] = {nullptr, };
		//std::vector<Item*> m_inventory;

};

#endif
