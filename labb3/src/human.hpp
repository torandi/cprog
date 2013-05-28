#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "character.hpp"
#include "equipment.hpp"

namespace game {
	class Keepable;

	class Human : public Character {
		public:

			//virtual void talk_to(Character * character);
			//drop
			//equip

		protected:
			Equipment * equipments[Equipment::NUM_SLOTS] = {nullptr, };
			std::set<Keepable*> m_inventory;

	};

};

#endif
