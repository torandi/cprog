#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "equipment.hpp"
#include "logging.hpp"

#include <sstream>

namespace game {

	Equipment::Equipment(
			const std::string & name, const std::string & description,
			int volume, int weight, type_t type,
			const std::map<std::string, int> effects)
		: Keepable(name, description, volume, weight)
		, m_effects(effects)
		, m_type(type) {
	}

	const std::map<std::string, int> &Equipment::effects() const {
		return m_effects;
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
		auto type_it = type_string_map.find((*node)["/type"].parse_string());
		if(type_it == type_string_map.end()) {
			Logging::fatal("Unknown equipment type %s", (*node)["/type"].parse_string().c_str());
		}

		type_t type = type_it->second;

		for(auto e : (*node)["/effects"].map()) {
			int val = e.second->parse_int();
			if(type == RING) val = std::max(0, val); // Clamp ring values to positive scale, allows us to use negative as "less probablity for effect"
			if(val > 0) { effects[e.first] = val; }
		}

		return new Equipment(
				(*node)["/name"].parse_string(),
				(*node)["/description"].parse_string(),
				(*node)["/volume"].parse_int(),
				(*node)["/weight"].parse_int(),
				type,
				effects
			);
	}

	std::string Equipment::description() const {
		std::stringstream str;
		str << Keepable::description() << std::endl;
		for(auto e : m_effects) {
			str << "\t" << e.first << ": " << e.second << std::endl;
		}
		return str.str();
	}

	std::map<std::string, Equipment::type_t> Equipment::type_string_map = {
		{"one_hand", Equipment::ONE_HAND},
		{"off_hand", Equipment::OFF_HAND},
		{"two_hand", Equipment::TWO_HAND},
		{"armor", Equipment::ARMOR},
		{"ring", Equipment::RING},
		{"backpack", Equipment::BACKPACK},
	};


}
