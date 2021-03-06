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
			virtual bool open(Character * character);
			virtual bool close(Character * character) { m_open = false; return true; };
      virtual bool aquire(Character * character) const { return false; };

			virtual int storage_volume() const;
			virtual int max_storage_volume() const;
			virtual std::string description() const;

			virtual const std::set<Keepable*> &content() const;
			virtual bool put(Keepable * item);
			virtual bool take(Keepable * item);


			std::set<Keepable*> content() { return m_content; };

			virtual bool is_open() const { return m_open; };

			virtual ~Container() {};

			virtual std::string name() const;

			static Container * from_config(const ConfigNode * node);
		protected:

			Container(const std::string &name, const std::string &description, int storage_volume);

			const int m_max_storage_volume;
			int m_storage_volume;
			std::set<Keepable*> m_content;

			bool m_open = false;
			std::string m_required_key = "";

	};
};

#endif
