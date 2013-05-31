#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "item.hpp"
#include "config.hpp"

#include <set>

namespace game {
	class Keepable;

	class Container : public Item {
		public:
			virtual bool pick_up(Character * character) const { return false; };
			virtual bool open(Character * character) { m_open = true; return true; };
			virtual bool close(Character * character) { m_open = false; return true; };
      virtual bool aquire(Character * character) const { return false; };

			virtual int storage_volume() const;
			virtual int max_storage_volume() const;

			virtual const std::set<Keepable*> &content() const;
			virtual bool put(Keepable * item);
			virtual bool take(Keepable * item);


			std::set<Keepable*> content() { return m_content; };

			virtual bool is_open() const { return m_open; };

			virtual ~Container() {};

			static Container * from_config(const ConfigNode * node);
		protected:

			Container(const std::string &name, const std::string &description, int storage_volume);

			const int m_max_storage_volume;
			int m_storage_volume;
			std::set<Keepable*> m_content;

			bool m_open = false;
	};
};

#endif
