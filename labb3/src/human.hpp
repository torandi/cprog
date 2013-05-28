#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "character.hpp"

namespace game {
	class Keepable;
	class Equipment;

	class Human : public Character {
		public:
			enum slot_t {
				RIGHT_HAND=0,
				LEFT_HAND,
				ARMOR,
				RING,
				BACKPACK,
				NUM_SLOTS
			};

			//virtual void talk_to(Character * character);
			//drop
			//equip

		protected:
			Equipment * equipments[NUM_SLOTS] = {nullptr, };
			std::set<Keepable*> m_inventory;

	};

};

#endif
