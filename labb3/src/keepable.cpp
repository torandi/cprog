#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "keepable.hpp"
#include "equipment.hpp"
#include "logging.hpp"

#include <sstream>

namespace game {
  Keepable::Keepable(const std::string & name, const std::string & description, int volume, int weight)
    : Item(name, description)
    , m_volume(volume)
    , m_weight(weight) {
  }

  int Keepable::volume() const {
    return m_volume;
  }

  int Keepable::weight() const {
    return m_weight;
  }

	Keepable * Keepable::from_config(const ConfigNode * node) {
		return new Keepable(
				(*node)["/name"].parse_string(),
				(*node)["/description"].parse_string(),
				(*node)["/volume"].parse_int(),
				(*node)["/weight"].parse_int()
			);
	}

	std::string Keepable::name() const {
		std::stringstream str;
		str << Item::name() << " [ Weight: " << weight() << "kg , Volume: " << volume() << " ]";
    return str.str();
	}
}
