#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <set>
#include <map>
#include <vector>
#include <functional>

#include "config.hpp"
#include "world_parser.hpp"

namespace game {
	class Character;
	class Item;
	class Keepable;

	class Area {
		public:

			static Area * from_config(const ConfigNode * node);

			virtual ~Area();

			virtual Area* neighbor(const std::string &direction) const;


			virtual const std::string &name() const;
			virtual const std::string &description() const;

			virtual int movement_cost() const;
			virtual bool enter(Character * character); /* Return false if action is denied */
			virtual bool leave(Character * character); /* Return false if action is denied */
			virtual void stay(Character * character); /* A character stays idle in the location */
			virtual const std::map<std::string, Area*> &exits() const;

      virtual const std::set<Character*> &characters() const;

			/* Returns all items acceable (including open containers) */
      virtual std::set<Item*> all_items() const;
			virtual const std::set<Item*> &items() const;

			virtual bool pick_up(Character * character, Item * item);
			virtual bool drop(Character * character, Item * item, bool force=true);

			void set_exits(const std::map<std::string, Area*> &exits);


		protected:
			Area(const std::string &name, const std::string &description, int movement_cost);

			const std::string m_name, m_description;
			const int m_movement_cost; /* Cost of leaving area */
			std::map<std::string, Area*> m_exits;
			std::vector<std::string> m_directions;

			std::set<Character*> m_characters;
			std::set<Item*> m_items;

      friend class WorldParser;
			friend class Game;
    private:
      static std::map<std::string, std::function<Area*(const ConfigNode*)>  > tag_map;
      static Area * self_from_config(const ConfigNode * node);
	};
};

#endif
