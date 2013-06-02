#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "item.hpp"
#include "keepable.hpp"
#include "equipment.hpp"
#include "container.hpp"
#include "unique_item.hpp"
#include "logging.hpp"
#include "health_potion.hpp"

namespace game {
	std::string Item::name() const {
		return raw_name();
	}
	std::string Item::description() const {
		return m_description;
	}

	Item::Item(const std::string &name, const std::string &description)
		: m_name(name)
		, m_description(description) {

	}

	Item * Item::from_node(const ConfigNode * node) {
		if(node->type == ConfigNode::NODE_MAPPING) {
			return from_config(node);
		} else if(node->type == ConfigNode::NODE_SCALAR) {
			std::string name = "/items/" + node->parse_string();
			Config cfg = Config::from_filename("game/items.yaml");
			return Item::from_config(&cfg[name]);
		} else {
			Logging::fatal("Item node must be mapping or a string naming the item id\n");
		}
	}

  Item * Item::from_config(const ConfigNode * node) {
    auto it = tag_map.find(node->tag());
    if(it == tag_map.end()) {
			node->print();
			Logging::fatal("Unknown item tag %s\n", node->tag().c_str());
		}
    return (it->second)(node);
  }

	bool Item::use(Character * character) const {
		Game::out(character->location()) << character->name() << " can't use " << name() << "." << std::endl;
		return false;
	}

  std::map<std::string, std::function<Item*(const ConfigNode*)> > Item::tag_map = {
    {"!keepable", &Keepable::from_config },
    {"!equipment", &Equipment::from_config },
    {"!container", &Container::from_config },
    {"!potion", &HealthPotion::from_config },
		{"!unique", &UniqueItem::from_config },
  };

}
