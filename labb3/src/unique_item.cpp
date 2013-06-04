#include "unique_item.hpp"
#include "color.hpp"
#include <sstream>

namespace game {
	UniqueItem::UniqueItem(const std::string &id, const std::string & name, const std::string & description, int volume, int weight)
		: Keepable(name, description, volume, weight)
		, m_id(id) {
	}

	const std::string &UniqueItem::id() {
		return m_id;
	}

	std::string UniqueItem::name() const {
		std::stringstream str;
		str << red;
		str << Keepable::raw_name() << normal;
		return str.str();
	}

	UniqueItem * UniqueItem::from_config(const ConfigNode * node) {
		return new UniqueItem(
				(*node)["/id"].parse_string(),
				(*node)["/name"].parse_string(),
				(*node)["/description"].parse_string(),
				(*node)["/volume"].parse_int(),
				(*node)["/weight"].parse_int()
			);
	}

}
