#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "container.hpp"

#include "keepable.hpp"
#include "logging.hpp"

namespace game {

	Container::Container(const std::string &name, const std::string &description, int storage_volume)
		: Item(name, description)
		, m_max_storage_volume(storage_volume) {
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
    const ConfigNode * content = node->find("/content");
    if(content != nullptr) {
      for(const ConfigNode * i : content->list()) {
        Keepable * item = Keepable::from_config(i);
        if(!container->put(item)) delete item;
      }
    }
    return container;
  }

}
