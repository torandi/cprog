#include "input.hpp"
#include "item.hpp"
#include "keepable.hpp"
#include "game.hpp"
#include "player.hpp"
#include "equipment.hpp"
#include "string_utils.hpp"
#include "area.hpp"

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

	static void print_characters(const std::set<const Character*> &characters) {
		for(const Character * c : characters) {
			std::cout << c->name() << std::endl;
		}
	}

	static std::set<const Character*> npcs() {
		std::set<const Character*> npcs = Game::player()->location()->characters();
		npcs.erase(Game::player());
		return npcs;
	}

	static Item * detect_item(std::set<Item*> items, const std::string &str) {
		Item * match = nullptr;
		size_t current_match_pos = str.npos;
		size_t current_match_len = 0;
		size_t pos;

		for(Item * i : items) {
			std::vector<std::string> item_name = split(Game::lowercase(i->name()), " ");
			size_t min_pos = str.npos;
			size_t match_len = 0;
			for(const std::string & n : item_name) {
				if((pos = str.find(n)) != str.npos) {
					min_pos = std::min(min_pos, pos);
					match_len += n.length();
				}
			}

			if(match_len > current_match_len) {
				current_match_pos = min_pos;
				current_match_len = match_len;
				match = i;
			} else if(current_match_len == match_len) {
				if(min_pos < current_match_pos) {
					current_match_pos = min_pos;
					current_match_len = match_len;
					match = i;
				}
			}
		}
		return match;
	}

	static const Character * detect_character(std::set<const Character*> characters, const std::string &str) {
		const Character * match = nullptr;
		size_t current_match_pos = str.npos;
		size_t current_match_len = 0;
		size_t pos;

		for(const Character * c : characters) {
			std::vector<std::string> character_name = split(Game::lowercase(c->name()), " ");
			size_t min_pos = str.npos;
			size_t match_len = 0;
			for(const std::string & n : character_name) {
				if((pos = str.find(n)) != str.npos) {
					min_pos = std::min(min_pos, pos);
					match_len += n.length();
				}
			}

			if(match_len > current_match_len) {
				current_match_pos = min_pos;
				current_match_len = match_len;
				match = c;
			} else if(current_match_len == match_len) {
				if(min_pos < current_match_pos) {
					current_match_pos = min_pos;
					current_match_len = match_len;
					match = c;
				}
			}
		}
		return match;
	}

	static void print_exits() {
		for(auto ex : Game::player()->location()->exits()) {
			std::cout << "To the " << ex.first << " lies " << ex.second->name() << "." << std::endl;
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

	static std::set<Item*> accessible_items() {
		std::set<Item*> items = Game::player()->location()->all_items();
		for(Keepable * i : Game::player()->inventory()) {
			items.insert(i);
		}
		return items;
	}

	static void cmd_look_around(ParseData &d) {
		Area * location =Game::player()->location();
		std::cout << "You are standing in " << location->name() << ". " << location->description() << "." << std::endl;
		print_exits();
		if(location->items().size() > 0) {
			std::cout << std::endl << "You can see the following item" << (location->items().size() > 1 ? "s" : "") << ": " << std::endl;
			print_items(location->items());
		}
		if(location->characters().size() > 1) {
			std::cout << std::endl << "You can see the following person" << (location->characters().size() > 2 ? "s" : "") << ": " << std::endl;
			print_characters(npcs());
		}
	}

	static void cmd_inspect(ParseData &d) {
		if(d.line.empty()) return cmd_look_around(d);
		const Character * character = detect_character(npcs(), d.line);

		if(character != nullptr) {
			std::cout << character->description() << std::endl;
		} else {
			Item * item = detect_item(accessible_items(), d.line);
			if(item != nullptr) {
				std::cout << "You take a closer look at " << item->name() << ". " << item->description() << std::endl;
			} else {
				std::cout << "You can't find any " << d.line << "." << std::endl;
			}
		}
	}

	static void cmd_go(ParseData &d) {
		if(Game::player()->go(d.line)) {
			std::cout << "You go " << d.line << ". Your are now in " << Game::player()->location()->name() << ". " << std::endl;
			print_exits();
		} else {
			std::cout << "You can't go that direction." << std::endl;
		}
	}

	/* Parse trees */

	ParseNode Input::parse_trees[Input::NUM_PARSE_TREES] = {
		ParseNode("", nullptr, {
			ParseNode("inventory", cmd_inventory, { }),
			ParseNode("attr", cmd_attributes, { }),
			ParseNode("equipment", cmd_equipment, { }),
			ParseNode("help", cmd_help_default, { }),
			ParseNode("look around", cmd_look_around, { }),
			ParseNode("describe", cmd_inspect, { }),
			ParseNode("inspect", cmd_inspect, { }),
			ParseNode("look at", cmd_inspect, { }),
			ParseNode("go", cmd_go, { }),
			ParseNode("next", [](ParseData &d) { Game::player()->next_turn(); }, { }),
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
