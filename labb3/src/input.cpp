#include "input.hpp"
#include "item.hpp"
#include "keepable.hpp"
#include "game.hpp"
#include "player.hpp"
#include "equipment.hpp"

#include <functional>
#include <set>

#include <readline/readline.h>
#include <readline/history.h>

using namespace std::placeholders;


namespace game {

	/* Helper mappings */

	static std::string slot_names[Human::NUM_SLOTS] = { "Right hand", "Left hand", "Armor", "Ring", "Backpack" };

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

	/* Functions */

	static void cmd_inventory(ParseData &d) {
		std::cout << "Your inventory contains: " << std::endl;
		print_items(Game::player()->inventory());
		std::cout << "--------------------------" << std::endl;
		std::cout << "Used space: " << Game::player()->used_backpack_volume()  << " / " << Game::player()->backpack_volume() << std::endl;
		std::cout << "Used weight: " << Game::player()->used_carrying_capacity() << "kg / " << Game::player()->carrying_capacity() << "kg" << std::endl;
	}

	static void cmd_attributes(ParseData &d) {
		std::cout << "Your attributes (including effects from equipment): " << std::endl;
		for(const auto it : Game::player()->attributes()) {
			std::cout << it.first << ": " << it.second << std::endl;
		}
		std::cout << "-----------" << std::endl <<
			"Damage bonus: " << Game::player()->extra_damage() << std::endl <<
			"Carrying capacity: " << Game::player()->used_carrying_capacity() << "kg / " << Game::player()->carrying_capacity() <<  "kg" << std::endl;
	}

	static void cmd_equipment(ParseData &d) {
		std::cout << "Your equipment: " << std::endl;
		for(int i=0; i<Human::NUM_SLOTS; ++i) {
			Equipment * e = Game::player()->equipment(static_cast<Human::slot_t>(i));
			if(e != nullptr) std::cout << slot_names[i] << ": " << e->name() << std::endl;
		}
	}

	static void cmd_help_default(ParseData &d) {
		std::cout << "Available commands: \n" << std::endl;
		for(const ParseNode & n : Input::parse_trees[Input::DEFAULT].children()) {
			std::cout << n.cmd() << std::endl;
		}
	}

	/* Parse trees */

	ParseNode Input::parse_trees[Input::NUM_PARSE_TREES] = {
		ParseNode("", nullptr, {
			ParseNode("inventory", cmd_inventory, { }),
			ParseNode("attr", cmd_attributes, { }),
			ParseNode("equipment", cmd_equipment, { }),
			ParseNode("help", cmd_help_default, { }),
		}),
		ParseNode("", nullptr, { })
	};

	void Input::init() {
	}

	void Input::cleanup() {

	}

	void Input::read(Input::parse_tree_t tree, const char * prompt, void * user_data) {
		char * line = nullptr;
		bool res;
		do {
			line = readline(prompt);
			if(line == nullptr) {
				Game::singleton->stop();
				return;
			}
			if(strlen(line) > 0) add_history(line);

			res  = ParseNode::parse(parse_trees[tree], std::string(line), user_data);
			if(!res && strlen(line) != 0) printf("Unknown command %s\n", line);
			free(line);
		} while(res == false);
	}

}
