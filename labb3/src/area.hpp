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


			virtual const std::string &name() const;
			virtual const std::string &description() const;

			virtual int movement_cost() const;
			virtual bool enter(Character * character); /* Return false if action is denied */
			virtual bool leave(Character * character); /* Return false if action is denied */
			virtual void stay(Character * character); /* A character stays idle in the location */

		protected:

			Area(const std::string &description, int movement_cost);

			const std::string m_name, m_description;
			const int m_movement_cost; /* Cost of leaving area */
			std::map<std::string, Area*> m_exits;
			std::vector<std::string> m_directions;

			std::set<const Character*> m_characters;
	};
};

#endif
