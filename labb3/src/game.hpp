#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>

namespace game {
	class Game {
		public:
			Game();
			~Game();

			enum relation_status_t {
				ALLIED,
				NEUTRAL,
				HOSTILE
			};

			void change_faction_relation(const std::string &faction1, const std::string &faction2, int mod) const;
			relation_status_t faction_relation(const std::string &faction1, const std::string &faction2) const;

		private:
			std::map<std::string, std::map<std::string, int*> > m_faction_relations;
	};
};

#endif
