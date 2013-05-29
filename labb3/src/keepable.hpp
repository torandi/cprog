#ifndef KEEPABLE_HPP
#define KEEPABLE_HPP

#include "item.hpp"
#include "config.hpp"

namespace game {

  class Keepable : public Item {
    public:

      virtual bool aquire(Character * character) const { return true; };
      virtual bool drop(Character * character) const { return true; };

      virtual int volume() const;
      virtual int weight() const;

      virtual const std::string &description() const;

      virtual ~Keepable() {};

      static Keepable * from_config(const ConfigNode * node);
    protected:
      Keepable(const std::string & name, const std::string & description, int volume, int weight);

      int m_volume, m_weight;

  };
};

#endif
