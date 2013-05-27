#ifndef ITEM_HPP
#define ITEM_HPP

namespace game {

	class Character;

	class Item {
		public:
			virtual bool pick_up(Character * character) const = 0; /* This should be call on by the area */
			virtual const std::string &name() const;
			virtual const std::string &description() const;

			~Item();
		protected:
			Item(const std::string &name, const std::string &description);

			const std::string m_name, m_description;
	};

};

#endif
