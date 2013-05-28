#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "character.hpp"
#include <set>

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


			virtual const std::set<Keepable*> &inventory() const;

			virtual int backpack_volume() const;
			virtual int carrying_capacity() const;
			int used_backpack_volume() const;
			int used_carrying_capacity() const;

			virtual int attribute(const std::string &attr) const;
			virtual bool talk_to(Human * human) { return false; };
			virtual bool drop(Keepable * item);
			virtual bool equip(Equipment * item);
			virtual bool equip(Equipment * item, slot_t slot);
			virtual bool unequip(slot_t slot);


			virtual ~Human();
		protected:
			Human(const std::string &name, const std::string &description, faction_t faction, std::map<std::string, int> attributes, Area * location);
			Equipment * equipments[NUM_SLOTS] = {nullptr, };
			std::set<Keepable*> m_inventory;

			int m_used_inventory_volume = 0;
			int m_inventory_weight = 0;

			virtual void store(Keepable * item);

	};

};

#endif
