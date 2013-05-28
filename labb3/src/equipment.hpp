#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

#include "keepable.hpp"

namespace game {

	class Equipment : public Keepable {
		public:
			enum type_t {
				ONE_HAND,
				OFF_HAND,
				TWO_HAND,
				ARMOR,
				RING,
				BACKPACK
			};

			virtual int effect(const std::string &attribute) const;
			virtual type_t type() const;

			virtual ~Equipment() {};
		protected:
			Equipment(const std::string & name, const std::string & description, int volume, int weight, type_t type, const std::map<std::string, int> effects);

			const std::map<std::string, int> m_effects;
			const type_t m_type;
	};

};

#endif
