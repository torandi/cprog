#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

namespace game {

	class Character;

	class Item {
		public:
			/* Return true if item can be picked up by character */
			virtual bool pick_up(Character * character) const { return true; };

			/*
			 * Returns true if item should be aquired, false if it should be deleted
			 */
			virtual bool aquire(Character * character) const = 0;
			virtual const std::string &name() const;
			virtual const std::string &description() const;

			virtual ~Item() {};
		protected:
			Item(const std::string &name, const std::string &description);

			const std::string m_name, m_description;
	};

};

#endif
