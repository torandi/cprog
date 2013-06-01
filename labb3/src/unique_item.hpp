#ifndef UNIQUE_ITEM
#define UNIQUE_ITEM

#include "keepable.hpp"

namespace game {
	class UniqueItem : public Keepable {
		public:
			virtual const std::string &id();
			virtual ~UniqueItem() { };

			static UniqueItem * from_config(const ConfigNode * node);
		protected:
			UniqueItem(const std::string &id, const std::string & name, const std::string & description, int volume, int weight);
			const std::string m_id;
	};
};

#endif
