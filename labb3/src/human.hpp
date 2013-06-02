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

			static std::string slot_names[NUM_SLOTS];


			virtual const std::set<Keepable*> &inventory() const;

			virtual int backpack_volume() const;
			virtual int carrying_capacity() const;
			int used_backpack_volume() const;
			int used_carrying_capacity() const;

			virtual std::string description() const;

      virtual void action();
      virtual void attack(Character * character, int points);
      virtual void attack(Character * character, int points, slot_t weapon_hand);
      virtual void incoming_attack(Character * character, int damage, bool critical_hit);
			virtual UniqueItem* have_unique(const std::string &id) const;

      virtual std::map<std::string, int> attributes() const;
			virtual int attribute(const std::string &attr, int default_value = -1) const;
			virtual Equipment * equipment(slot_t slot) const;
			virtual bool talk_to(Human * human);
			virtual bool drop(Keepable * item);
			virtual bool equip(Equipment * item);
			virtual bool equip(Equipment * item, slot_t slot);
			virtual bool unequip(slot_t slot);

			static Human * from_config(const ConfigNode * node, Area * location);
			static void parse_equipment(Human * human, const ConfigNode * node);
			virtual void init_round();

			virtual ~Human();
		protected:
			Human(const std::string &name, const std::string &description, faction_t faction, std::map<std::string, int> attributes, Area * location);
			Equipment * m_equipments[NUM_SLOTS] = {nullptr, };
			std::set<Keepable*> m_inventory;

			int m_used_inventory_volume = 0;
			int m_inventory_weight = 0;


			std::vector<std::string> m_dialog;
			int m_next_dialog = 0;

			virtual void store(Keepable * item);
			virtual void die();
			virtual void reduce_armor(int amount);

			virtual bool block(int damage, int points, slot_t weapon_hand);
      virtual int apply_block_damage(slot_t slot, int damage, Equipment * weapon);
      virtual bool passive_protection(Character * character, int damage);

	};

};

#endif
