#include "darkwood.hpp"
#include "character.hpp"


namespace game {

	Darkwood::Darkwood(const std::string &name, const std::string &description, int movement_cost) :
		Area(name, description, movement_cost) { }

	bool Darkwood::enter(Character * character) {
		if(character->have_unique("darkwood_map") != nullptr) {
			return Area::enter(character);
		} else {
			std::cout << "Entering the Darkwoods without a map is a garanteed death sentence. Find a map before you enter." << std::endl;
			return false;
		}
	}

	Darkwood * Darkwood::from_config(const ConfigNode * _node) {
		const ConfigNode &node = *_node;
		std::string name = node["/name"].parse_string();
		std::string description = node["/description"].parse_string();
		int movement_cost = node["/movement_cost"].parse_int();
		return new Darkwood(name, description, movement_cost);
	}
}
