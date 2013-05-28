#include "keepable.hpp"

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
}
