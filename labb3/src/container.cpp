#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "container.hpp"

#include "keepable.hpp"
#include "logging.hpp"
#include "item.hpp"
#include "unique_item.hpp"
#include "character.hpp"
#include "player.hpp"
#include <sstream>

namespace game {

	Container::Container(const std::string &name, const std::string &description, int storage_volume)
		: Item(name, description)
		, m_max_storage_volume(storage_volume)
		, m_storage_volume(storage_volume) {
	}


	int Container::storage_volume() const {
		return m_storage_volume;
	}

	int Container::max_storage_volume() const {
		return m_max_storage_volume;
	}

	const std::set<Keepable*> &Container::content() const {
		return m_content;
	}

	bool Container::put(Keepable * item) {
		if(m_storage_volume >= item->volume()) {
			auto res = m_content.insert(item);
			if(res.second) {
				m_storage_volume -= item->volume();
				return true;
			} else {
				Logging::fatal("Duplicate item %s in container %s\n", item->name().c_str(), name().c_str());
			}
		} else {
			return false;
		}
	}

	bool Container::take(Keepable * item) {
		auto it = m_content.find(item);
		if(it != m_content.end()) {
			m_content.erase(it);
			m_storage_volume += item->volume();
			return true;
		} else {
			return false;
		}
	}

  Container * Container::from_config(const ConfigNode * node) {
    Container * container = new Container(
        (*node)["/name"].parse_string(),
        (*node)["/description"].parse_string(),
        (*node)["/storage_volume"].parse_int()
      );
		const ConfigNode * key_node  = node->find("/key");
		if(key_node != nullptr) {
			container->m_required_key = key_node->parse_string();
		}
    const ConfigNode * content = node->find("/content");
    if(content != nullptr) {
      for(const ConfigNode * i : content->list()) {
        Keepable * item = dynamic_cast<Keepable*>(Item::from_node(i));
        if(item != nullptr) {
          if(!container->put(item)) delete item;
        }
      }
    }
    return container;
  }

	std::string Container::description() const {
		if(m_open) {
			std::stringstream str;
			str << Item::description() << " It's open";
			if(m_content.size() > 0) {
				str << ", and you see:" << std::endl;
				for(Keepable * item : m_content) {
					str << item->name() << std::endl;
				}
			} else {
				str << " and there seems to be nothing inside.";
			}
			return str.str();
		} else {
			return Item::description();
		}
	}

	bool Container::open(Character * character) {

		if(!m_open && !m_required_key.empty()) {
			UniqueItem * item = character->have_unique(m_required_key);
			if (item != nullptr) {
				Game::out(character->location()) << character->name() << " " << character->verb("use") << " " << item->raw_name() << " to open " << name() << "." << std::endl;
			} else if(character == Game::player()) {
				std::cout << "It's locked." << std::endl;
				return false;
			} else return false;
		}
		m_open = true;
		return true;
	}

}
