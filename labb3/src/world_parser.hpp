#ifndef WORLD_PARSER_HPP
#define WORLD_PARSER_HPP

namespace game {
	class Game;

	class WorldParser {
		public:
			static void parse(Game * game);
		private:
			static void parse_areas(Game * game);
	};
};

#endif
