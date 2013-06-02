#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "equipment.hpp"
#include "logging.hpp"
#include "world_parser.hpp"
#include "color.hpp"

#include <sstream>

namespace game {

	Equipment::Equipment(
			const std::string & name, const std::string & description,
			int volume, int weight, type_t type, rarity_t rarity,
			const std::map<std::string, int> effects)
		: Keepable(name, description, volume, weight)
		, m_effects(effects)
		, m_type(type)
    , m_rarity(rarity) {
	}

	const std::map<std::string, int> &Equipment::effects() const {
		return m_effects;
	}

  void Equipment::reduce(const std::string &attribute, int amount) {
    auto it = m_effects.find(attribute);
    if(it != m_effects.end()) {
      m_effects[attribute] = std::max(0, it->second - amount);
    }
  }

	int Equipment::effect(const std::string &attr, int default_value) const {
		auto it = m_effects.find(attr);
		if(it != m_effects.end()) {
			return it->second;
		} else {
			return default_value;
		}
	}

	Equipment::type_t Equipment::type() const {
		return m_type;
	}

  Equipment::rarity_t Equipment::rarity() const {
    return m_rarity;
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
    type_t type = map_type((*node)["/type"].parse_string());

		for(auto e : (*node)["/effects"].map()) {
			int val = e.second->parse_int();
			if(type == RING) val = std::max(0, val); // Clamp ring values to positive scale, allows us to use negative as "less probablity for effect"
			if(val > 0) { effects[e.first] = val; }
		}

    std::string name = (*node)["/name"].parse_string();

    /* Generate prefixes */
    std::vector<WorldParser::item_prefix_t> prefixes = WorldParser::generate_prefixes(type);

    rarity_t rarity = static_cast<rarity_t>(prefixes.size());

    for(WorldParser::item_prefix_t & pfx : prefixes) {
      name = pfx.name + " " + name;
      for(auto attr : pfx.attributes) {
        effects[attr.first] += attr.second;
      }
    }

		return new Equipment(
				name,
				(*node)["/description"].parse_string(),
				(*node)["/volume"].parse_int(),
				(*node)["/weight"].parse_int(),
				type,
        rarity,
				effects
			);
	}

  std::string Equipment::raw_name() const {
    std::stringstream str;
    switch(m_rarity) {
      case MAGIC:
        str << blue;
        break;
      case RARE:
        str << yellow;
        break;
      case LEGENDARY:
        str << purple;
        break;
      default:
        break;
      }
    str << Keepable::raw_name() << normal;
    return str.str();
  }

	std::string Equipment::description() const {
		std::stringstream str;
		str << Keepable::raw_name() << ": " << Keepable::description() << std::endl;
		for(auto e : m_effects) {
			str << "\t" << e.first << ": " << e.second << std::endl;
		}
		return str.str();
	}

  Equipment::type_t Equipment::map_type(const std::string &name) {
    auto type_it = type_string_map.find(name);
    if(type_it == type_string_map.end()) {
      Logging::fatal("Unknown equipment type %s", name.c_str());
    }

    return type_it->second;
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
