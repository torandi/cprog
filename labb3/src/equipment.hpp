#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP


#include <string>
#include <map>

#include "config.hpp"
#include "keepable.hpp"
#include "human.hpp"

namespace game {

	class Equipment : public Keepable {
		public:
			enum type_t {
				ONE_HAND=0,
				TWO_HAND,
        OFF_HAND,
				ARMOR,
				RING,
				BACKPACK
			};


			virtual std::string description() const;
			virtual const std::map<std::string, int> &effects() const;
			virtual int effect(const std::string &attribute, int default_value=0) const;
			virtual std::string item_type() const { return "equipment"; } ;
			virtual type_t type() const;

      virtual void reduce(const std::string &attribute, int amount);


			virtual ~Equipment() {};

			static Human::slot_t default_slot(type_t type);

			static Equipment * from_config(const ConfigNode * node);
		protected:
			Equipment(const std::string & name, const std::string & description, int volume, int weight, type_t type, const std::map<std::string, int> effects);

			std::map<std::string, int> m_effects;
			const type_t m_type;

			static std::map<std::string, type_t> type_string_map;
	};

};

#endif
