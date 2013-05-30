#ifndef WORLD_PARSER_HPP
#define WORLD_PARSER_HPP

#include <map>
#include <string>
#include "item.hpp"
#include "config.hpp"
#include <random>

namespace game {
	class Game;

	class WorldParser {
		public:
			static void parse(Game * game);
		private:

			static void parse_areas(Game * game);
			static void parse_items(Game * game);
      static std::default_random_engine generator;

	};
};

#endif
