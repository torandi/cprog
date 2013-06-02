#ifndef WORLD_PARSER_HPP
#define WORLD_PARSER_HPP

#include <map>
#include <string>
#include "config.hpp"
#include "equipment.hpp"
#include <random>

namespace game {
	class Game;
  class Keepable;

	class WorldParser {
		public:
      static const ConfigNode * current_prefix_probability;

			static void parse(Game * game);

      static std::vector<Keepable*> random_items(const std::vector<const ConfigNode*> &items, int count);

      struct item_prefix_t {
        std::string name;
        std::map<std::string, int> attributes;
        enum type_t { PREFIX, SUFFIX } type;
      };


      static std::vector<item_prefix_t> generate_prefixes(Equipment::type_t type);
		private:


			static void parse_areas(Game * game);
			static void parse_items(Game * game);
      static std::default_random_engine generator;

      /* first lvl: type of equipment; 2nd: group; 3rd: prefixes */
      static std::vector<std::vector<item_prefix_t> > prefix_table[Equipment::NUM_TYPES];
      static std::vector<std::discrete_distribution<int> > prefix_distribution_table[Equipment::NUM_TYPES];

	};
};

#endif
