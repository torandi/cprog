#include "keepable.hpp"

namespace game {
  Keepable::Keepable(const std::string & name, const std::string & description, int size, int weight)
    : Item(name, description)
    , m_size(size)
    , m_weight(weight) {
  }

  int Keepable::size() const {
    return m_size;
  }

  int Keepable::weight() const {
    return m_weight;
  }
}
