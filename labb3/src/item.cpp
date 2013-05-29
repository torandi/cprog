#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "item.hpp"
#include "keepable.hpp"
#include "equipment.hpp"
#include "container.hpp"
#include "logging.hpp"
#include "health_potion.hpp"

namespace game {
	const std::string &Item::name() const {
		return m_name;
	}
	const std::string &Item::description() const {
		return m_description;
	}

	Item::Item(const std::string &name, const std::string &description)
		: m_name(name)
		, m_description(description) {

	}

  Item * Item::from_config(const ConfigNode * node) {
    auto it = tag_map.find(node->tag());
    if(it == tag_map.end()) Logging::fatal("Unknown keepable tag %s\n", node->tag().c_str());
    return (it->second)(node);
  }

  std::map<std::string, std::function<Item*(const ConfigNode*)> > Item::tag_map = {
    {"!keepable", &Keepable::from_config },
    {"!equipment", &Equipment::from_config },
    {"!container", &Container::from_config },
    {"!potion", &HealthPotion::from_config },
  };
}
