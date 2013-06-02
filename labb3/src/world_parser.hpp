#ifndef WORLD_PARSER_HPP
#define WORLD_PARSER_HPP

#include <map>
#include <string>
#include "config.hpp"
#include <random>

namespace game {
	class Game;
  class Keepable;

	class WorldParser {
		public:
			static void parse(Game * game);

      static std::vector<Keepable*> random_items(const std::vector<const ConfigNode*> &items, int count);
		private:

			static void parse_areas(Game * game);
			static void parse_items(Game * game);
      static std::default_random_engine generator;

	};
};

#endif
