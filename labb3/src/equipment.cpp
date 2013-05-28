#include "equipment.hpp"

namespace game {

	Equipment::Equipment(
			const std::string & name, const std::string & description,
			int volume, int weight, type_t type,
			const std::map<std::string, int> effects)
		: Keepable(name, description, volume, weight)
		, m_effects(effects)
		, m_type(type) {
	}

	int Equipment::effect(const std::string &attr) const {
		auto it = m_effects.find(attr);
		if(it != m_effects.end()) {
			return it->second;
		} else {
			return 0;
		}
	}

	Equipment::type_t Equipment::type() const {
		return m_type;
	}

}
