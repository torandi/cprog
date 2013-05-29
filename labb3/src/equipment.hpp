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
				ONE_HAND,
				OFF_HAND,
				TWO_HAND,
				ARMOR,
				RING,
				BACKPACK
			};


			virtual const std::map<std::string, int> &effects() const;
			virtual int effect(const std::string &attribute) const;
			virtual type_t type() const;


			virtual ~Equipment() {};

			static Human::slot_t default_slot(type_t type);

			static Equipment * from_config(const ConfigNode * node);
		protected:
			Equipment(const std::string & name, const std::string & description, int volume, int weight, type_t type, const std::map<std::string, int> effects);

			const std::map<std::string, int> m_effects;
			const type_t m_type;

			static std::map<std::string, type_t> type_string_map;
	};

};

#endif
