#ifndef REQUIRING_CHARACTER
#define REQUIRING_CHARACTER

#include "character.hpp"
#include "config.hpp"

#include <vector>
#include <string>

namespace game {
	class Keepable;
	class Area;

	/* A character that requires a item for another item. Can't die */
	class RequiringCharacter : public Character {
		public:
			virtual void action() {};

			virtual void attack(Character * character, int points) {};

			static RequiringCharacter * from_config(const ConfigNode * node, Area * location);

			virtual void incoming_attack(Character * character, int damage, bool critical_hit);

			virtual ~RequiringCharacter() {};

			virtual bool talk_to(Character * character);
			virtual void delete_item(Keepable  * item) {};
			virtual void store(Keepable * item) {};
		protected:
			RequiringCharacter(const std::string &name, const std::string &description, Area * location, const std::string &required_item, Keepable * give_item);

			virtual void die() {};
			virtual void reduce_armor(int amount) {};

			std::string m_required_item;
			Keepable * m_give_item;
			bool m_given_item = false;
			int m_next_dialog = 0;

			std::vector<std::string> m_require_dialog;
			std::string m_give_dialog;
			std::string m_no_more_dialog;
	};
};

#endif
