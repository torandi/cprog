#ifndef DARKWOOD_HPP
#define DARKWOOD_HPP

#include "area.hpp"
#include "config.hpp"

namespace game {
	class Darkwood : public Area {
		public:
			static Darkwood * from_config(const ConfigNode * node);
			virtual ~Darkwood() {};
			virtual bool enter(Character * character); /* Return false if action is denied */
		protected:
			Darkwood(const std::string &name, const std::string &description, int movement_cost);

	};
};

#endif
