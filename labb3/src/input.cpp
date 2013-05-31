#include "input.hpp"
#include "item.hpp"
#include "keepable.hpp"
#include "game.hpp"
#include "player.hpp"

#include <functional>
#include <set>

using namespace std::placeholders;


namespace game {

	/* Helper functions */

	static void print_items(const std::set<Item*> &items) {
		for(Item * i : items) {
			std::cout << i->name() << std::endl;
		}
	}

	static void print_items(const std::set<Keepable*> &items) {
		for(Keepable * i : items) {
			std::cout << i->name() << std::endl;
		}
	}

	/* Parse trees */

	ParseNode Input::parse_trees[Input::NUM_PARSE_TREES] = {
		ParseNode("", nullptr, {
			ParseNode("inventory", [](ParseData d) { 
				std::cout << "Inventory: " << std::endl;
				print_items(Game::player()->inventory());
			}, { })
		}),
		ParseNode("", nullptr, { })
	};

	void Input::init() {
	}

	void Input::cleanup() {

	}

	void Input::read(Input::parse_tree_t tree, void * user_data) {

	}

}
