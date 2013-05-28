#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "item.hpp"
#include "config.hpp"

#include <set>

namespace game {
	class Keepable;

	class Container : public Item {
		public:
			virtual bool pick_up(Character * character) const { return true; };
			virtual bool open(Character * character) const { return true; };

			virtual int storage_volume() const;
			virtual int max_storage_volume() const;

			virtual const std::set<Keepable*> &content() const;
			virtual bool put(Keepable * item);
			virtual bool take(Keepable * item);

			virtual ~Container() {};

			static Container * from_config(ConfigNode * node);
		protected:

			Container(const std::string &name, const std::string &description, int storage_volume);

			const int m_max_storage_volume;
			int m_storage_volume;
			std::set<Keepable*> m_content;
	};
};

#endif
