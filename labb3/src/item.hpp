#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <functional>
#include "config.hpp"

namespace game {

	class Character;

	class Item {
		public:
			/* Return true if item can be picked up by character */
			virtual bool pick_up(Character * character) const { return true; };

			/*
			 * Returns true if item should be aquired, false if it should be deleted
			 */
			virtual bool aquire(Character * character) const = 0;
      virtual std::string name() const;
			virtual std::string description() const;

      std::string raw_name() const { return m_name; };

			virtual bool use(Character * character) const;

			virtual ~Item() { };

      static Item * from_config(const ConfigNode * node);

			/* Node of either full item, or a id in game/items.yaml */
			static Item * from_node(const ConfigNode * node);
		protected:
			Item(const std::string &name, const std::string &description);

			const std::string m_name, m_description;

    private:
      static std::map<std::string, std::function<Item*(const ConfigNode*)> > tag_map;
	};

};

#endif
