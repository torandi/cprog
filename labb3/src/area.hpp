#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <set>
#include <map>
#include <vector>

namespace game {
	class Character;

	class Area {
		public:

			virtual const std::vector<std::string> &directions() const;
			virtual Area* neighbor(const std::string &direction) const;

			virtual const std::string &description() const;

			virtual void enter(Character * character);
			virtual void leave(Character * character);
			virtual void stay(Character * character); /* A character stays idle in the location */

		protected:

			Area(const std::string &description);

			const std::string m_description;
			std::map<std::string, Area*> m_exits;
			std::vector<std::string> m_directions;

			std::set<const Character*> m_characters;
	};
};

#endif
