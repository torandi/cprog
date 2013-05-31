#include "input.hpp"
#include "item.hpp"
#include "keepable.hpp"
#include "game.hpp"
#include "player.hpp"
#include "equipment.hpp"
#include "string_utils.hpp"
#include "area.hpp"
#include "container.hpp"

#include <functional>
#include <set>
#include <cstring>

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

	static std::set<Item*> accessible_items() {
		std::set<Item*> items = Game::player()->location()->all_items();
		for(Keepable * i : Game::player()->inventory()) {
			items.insert(i);
		}
		return items;
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

	static Item * detect_item(std::set<Keepable*> items, const std::string &str) {
		std::set<Item*> new_items;
		for(Keepable * k : items) {
			new_items.insert(k);
		}
		return detect_item(new_items, str);
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

	static std::set<Item*> filter_list(const std::set<Item*> & list, std::function<Item*(Item*)> func) {
		std::set<Item*> ret;
		for(Item * i : list) {
			Item * t = func(i);
			if(t != nullptr) ret.insert(i);
		}
		return ret;
	}

	static Item * find_item_and_container(const std::string &str, Container ** container) {
		*container = nullptr;
		Item * i = detect_item(accessible_items(), str);
		if(i != nullptr) {
			if(Game::player()->location()->items().count(i) == 0) {
				/* from chest */
				Keepable * k = dynamic_cast<Keepable*>(i);
				if(k != nullptr) {
					std::set<Item*> chests = filter_list(Game::player()->location()->items(), [](Item* i){
							return dynamic_cast<Container*>(i);
							});
					for(Item * c : chests) {
						Container * chest = dynamic_cast<Container*>(c);
						if(chest->content().count(k) == 1) *container = chest;
					}
				}
			}
		}
		return i;
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
			if(n.cmd() == "make") continue;
			std::cout << n.cmd() << std::endl;
		}
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
				std::cout << item->description() << std::endl;
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

	static void cmd_pick_up(ParseData &d) {
		Container * c= nullptr;
		Item * i = find_item_and_container(d.line, &c);

		if(i != nullptr && c == nullptr) {
				Game::player()->pick_up(i);
		} else if(i != nullptr) {
			Game::player()->take(dynamic_cast<Keepable*>(i), c);
		} else {
			std::cout << "You try to find some " << d.line << " to pick up, but you don't find any." << std::endl;
		}
	}

	static void cmd_take_from(ParseData &d) {
		Container * c = dynamic_cast<Container*>(detect_item(filter_list(Game::player()->location()->items(), [](Item*i){
						return dynamic_cast<Container*>(i);
			}), d.line));
		if(c != nullptr) {
			Keepable * i = dynamic_cast<Keepable*>(detect_item(c->content(), d.line));
			if(i != nullptr) {
				Game::player()->take(i, c);
			} else {
				std::cout << "You can't find that in " << c->name() << "." << std::endl;
			}
		} else {
			std::cout << "What from what did you say?" << std::endl;
		}
	}

	static void cmd_open(ParseData &d) {
		Item * i = detect_item(Game::player()->location()->items(), d.line);
		Container * c = dynamic_cast<Container*>(i);
		if(c != nullptr) {
			if(c->open(Game::player())) {
				std::cout << "You open " << c->name() << ". Inside you see: " << std::endl;
				print_items(c->content());
			} else {
				std::cout << "You try to open " << c->name() << " but the lid just won't give way." << std::endl;
			}
		} else {
			std::cout << "You can't find any " << d.line << " to open." << std::endl;
		}
	}

	static void cmd_close(ParseData &d) {
		Item * i = detect_item(Game::player()->location()->items(), d.line);
		Container * c = dynamic_cast<Container*>(i);
		if(c != nullptr) {
			if(c->close(Game::player())) std::cout << "You close " << c->name() << "." << std::endl;
		} else {
			std::cout << "You can't find any " << d.line << " to open." << std::endl;
		}
	}

	static void cmd_equip(ParseData &d) {
		Container * c = nullptr;
		Item * i = find_item_and_container(d.line, &c);
		if(i != nullptr) {
			Equipment * e = dynamic_cast<Equipment*>(i);
			if(e != nullptr) {
				Human::slot_t slot = Equipment::default_slot(e->type());
				if(d.line.find("right")) slot = Human::RIGHT_HAND;
				else if(d.line.find("left")) slot = Human::LEFT_HAND;
				if(Game::player()->equip(e, slot)) {
					c->take(e);
					std::cout << "You equipped " << e->name() << "." << std::endl;
				}
			} else {
				std::cout << "You try to equip " << i->name() << " a couple of times, but fail misserably." << std::endl;
			}
		}
	}

	static void cmd_use(ParseData &d) {
		Item * i = detect_item(accessible_items(), d.line);
		if(i != nullptr) {
			i->use(Game::player());
		} else {
			std::cout << "You are sure that you saw a " << d.line << " to use just a moment ago, but it seems to be totaly gone. Or you know, maybe you should learn to type." << std::endl;
		}
	}

	static void cmd_make(ParseData &d) {
		std::cout << "Suck. Hur svårt ska det vara. Avsluta innan du bygger." << std::endl;
		system("make");
		Game::singleton->stop();
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
			ParseNode("open", cmd_open, { }),
			ParseNode("close", cmd_close, { }),
			ParseNode("next", [](ParseData &d) { Game::player()->next_turn(); }, { }),
			ParseNode("take", nullptr, {
				ParseNode("from", cmd_take_from, {}),
				ParseNode("", cmd_pick_up, {}),
			}),
			ParseNode("pick up", nullptr, {
				ParseNode("from", cmd_take_from, {}),
				ParseNode("", cmd_pick_up, {}),
			}),
			ParseNode("equip", cmd_equip, { }),
			ParseNode("use", cmd_use, { }),
			ParseNode("make", cmd_make, { }),
		}),
		ParseNode("", nullptr, { })
	};

	static std::vector<std::string> commands[Input::NUM_PARSE_TREES];
	static Input::parse_tree_t active_parse_tree;

	char *cmd_completion(const char *text, int state) {
		static size_t last_index, len;
		static std::string lcased;
		if(!state) {
			lcased = Game::lowercase(std::string(text));
			last_index = 0;
			len = strlen(text);
		}

		while(last_index < commands[active_parse_tree].size()) {
			std::string name = commands[active_parse_tree][last_index++];
			if(name == "make") continue;
			if(strncmp(name.c_str(), lcased.c_str(), len) == 0) return strdup(name.c_str());
		}

		return nullptr;
	}

	char *stuff_completion(const char *text, int state) {
		static size_t last_index, len;
		static std::string lcased;
		static std::vector<std::string> names;
		if(!state) {
			lcased = Game::lowercase(std::string(text));
			last_index = 0;
			len = strlen(text);
			for(const auto & exit : Game::player()->location()->exits()) {
				names.push_back(Game::lowercase(exit.first));
			}
			for(Item * i : accessible_items()) {
				names.push_back(Game::lowercase(i->name()));
			}
			for(const Character * c : npcs()) {
				names.push_back(Game::lowercase(c->name()));
			}
		}

		while(last_index < names.size()) {
			std::string name = names[last_index++];
			if(strncmp(name.c_str(), lcased.c_str(), len) == 0) return strdup(name.c_str());
		}

		return nullptr;
	}

	static char ** completion_func(const char * text, int start, int end) {
		char ** matches = NULL;
		if(start == 0) {
			matches = rl_completion_matches(text, cmd_completion);
		} else {
			matches = rl_completion_matches(text, stuff_completion);
		}
		return matches;
	}

	void Input::init() {
		for(int tree=0; tree < NUM_PARSE_TREES; ++tree) {
			for(const ParseNode & n : Input::parse_trees[tree].children()) {
				commands[tree].push_back(Game::lowercase(n.cmd()));
			}
		}
		rl_attempted_completion_function = completion_func;
	}

	void Input::cleanup() {

	}

	void Input::read(Input::parse_tree_t tree, const char * prompt, void * user_data) {
		char * line = nullptr;
		bool res;
		active_parse_tree = tree;
		do {
			line = readline(prompt);

			if(line == nullptr) {
				Game::singleton->stop();
				return;
			}
			if(strlen(line) > 0) add_history(line);

			std::string cmd = std::string(line);
			free(line);

			/* Fulhack alias för nswe */
			if(cmd.length() == 1) {
				switch(cmd[0]) {
					case 'n':
						cmd = "go north";
						break;
					case 's':
						cmd = "go south";
						break;
					case 'w':
						cmd = "go west";
						break;
					case 'e':
						cmd = "go east";
						break;
					default:
						break;
				}
			}

			res  = ParseNode::parse(parse_trees[tree], cmd, user_data);
			if(!res && cmd.length() != 0) printf("Unknown command %s.\n", cmd.c_str());
		} while(res == false);
	}

}
