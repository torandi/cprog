#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "equipment.hpp"
#include "logging.hpp"

namespace game {

	Equipment::Equipment(
			const std::string & name, const std::string & description,
			int volume, int weight, type_t type,
			const std::map<std::string, int> effects)
		: Keepable(name, description, volume, weight)
		, m_effects(effects)
		, m_type(type) {
	}

	int Equipment::effect(const std::string &attr) const {
		auto it = m_effects.find(attr);
		if(it != m_effects.end()) {
			return it->second;
		} else {
			return 0;
		}
	}

	Equipment::type_t Equipment::type() const {
		return m_type;
	}

	Human::slot_t Equipment::default_slot(Equipment::type_t type) {
		switch(type) {
			case ONE_HAND: return Human::RIGHT_HAND;
			case OFF_HAND: return Human::LEFT_HAND;
			case TWO_HAND: return Human::RIGHT_HAND;
			case ARMOR: return Human::ARMOR;
			case RING: return Human::RING;
			case BACKPACK: return Human::BACKPACK;
			default:
				Logging::fatal("Unknown equipment type");
		}
	}

	Equipment * Equipment::from_config(const ConfigNode * node) {
		std::map<std::string, int> effects;
		auto type = type_string_map.find((*node)["/type"].parse_string());
		if(type == type_string_map.end()) {
			Logging::fatal("Unknown equipment type %s", (*node)["/type"].parse_string().c_str());
		}
		return new Equipment(
				(*node)["/name"].parse_string(),
				(*node)["/description"].parse_string(),
				(*node)["/volume"].parse_int(),
				(*node)["/weight"].parse_int(),
				type->second,
				effects
			);
	}

	std::map<std::string, Equipment::type_t> Equipment::type_string_map = {
		{"one_hand", Equipment::ONE_HAND},
		{"off_hand", Equipment::OFF_HAND},
	};


}
