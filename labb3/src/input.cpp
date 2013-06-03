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
#include <climits>

#include <readline/readline.h>
#include <readline/history.h>

using namespace std::placeholders;


namespace game {

	static int default_attack = INT_MIN;
	static int default_block = INT_MIN;

	static std::string redo = "";

	/* Helper mappings */

	static std::string slot_names[Human::NUM_SLOTS] = { "Right hand", "Left hand", "Armor", "Ring", "Backpack" };

	/* Helper functions */

	static void print(const std::string &str) {
		std::cout << str << std::endl;
	}

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

	static void print_characters(const std::set<Character*> &characters) {
		for(Character * c : characters) {
			std::cout << c->name() << std::endl;
		}
	}

	static int find_number(const std::string &line) {
		size_t pos = line.find_first_of("0123456789");
		if(pos == line.npos) {
			return INT_MIN;
		} else {
			return atoi(line.c_str() + pos);
		}
	}


	static std::set<Character*> npcs() {
		std::set<Character*> npcs = Game::player()->location()->characters();
		npcs.erase(Game::player());
		return npcs;
	}

	static std::set<Item*> accessible_items(bool include_equipped=true) {
		std::set<Item*> items = Game::player()->location()->all_items();
		for(Keepable * i : Game::player()->inventory()) {
			items.insert(i);
		}
		if(include_equipped) {
			for(int i=0; i<Human::NUM_SLOTS; ++i) {
				if(Game::player()->equipment(static_cast<Human::slot_t>(i)) != nullptr) items.insert(Game::player()->equipment(static_cast<Human::slot_t>(i)));
			}
		}
		return items;
	}

	/* Count is used when multiple items match */
	static Item * detect_item(std::set<Item*> items, const std::string &str, int count=INT_MIN) {
		Item * match = nullptr;
		size_t current_match_pos = str.npos;
		size_t current_match_len = 0;
		size_t pos;

		for(Item * i : items) {
			std::vector<std::string> item_name = split(Game::lowercase(i->raw_name()), " ");
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

		if(match != nullptr) {
			if(count < 0) count = find_number(str);
			if(count > 1) {
				items.erase(match);
				Item * m2 = detect_item(items, str, count - 1);
				if(m2 != nullptr) match = m2;
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

	static Character * detect_character(std::set<Character*> characters, const std::string &str) {
		Character * match = nullptr;
		size_t current_match_pos = str.npos;
		size_t current_match_len = 0;
		size_t pos;

		for(Character * c : characters) {
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

	static Item * find_item_and_container(std::set<Item*> items,const std::string &str, Container ** container) {
		*container = nullptr;
		Item * i = detect_item(items, str);
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
		Character * c = Game::player();
		/*c = detect_character(npcs(), d.line);
		if(c == nullptr) c = Game::player();*/


		std::cout << "Your attributes (including effects from equipment): " << std::endl;
		for(const auto it : c->attributes()) {
			std::cout << it.first << ": " << it.second << std::endl;
		}
		std::cout << "-----------" << std::endl <<
			"Armor protection: " << Game::player()->armor_protection() << std::endl <<
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

  void Input::describe_area() {
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

	static void cmd_look_around(ParseData &d) {
    Input::describe_area();
	}

	static void cmd_inspect(ParseData &d) {
		if(d.line.empty()) return cmd_look_around(d);
		Character * character = detect_character(npcs(), d.line);

		if(character != nullptr) {
			std::cout << character->description() << std::endl;
		} else {
			Item * item = detect_item(accessible_items(), d.line);
			if(item != nullptr) {
				//std::cout << "You take a closer look at " << item->name() << std::endl;
				std::cout << item->description() << std::endl;
			} else {
				std::cout << "You can't find any " << d.line << "." << std::endl;
			}
		}
	}

	static void cmd_talk_to(ParseData &d) {
		if(d.line.empty()) return print("Talk to whom?!");

		Character * character = detect_character(npcs(), d.line);

		if(character != nullptr) {
			if(!character->talk_to(Game::player())) {
				std::cout << character->name() << " refuses to talk to you!" << std::endl;
			}
		} else {
			std::cout << "You would really like to talk to" << d.line << ", but since you can't find him/her/it you simply have to give up." << std::endl;
		}
	}

	static void cmd_go(ParseData &d) {
		if(d.line.empty()) return print("Go where?!");
		if(Game::player()->go(d.line)) {
			std::cout << "You go " << d.line << ". Your are now in " << Game::player()->location()->name() << ". "  << Game::player()->location()->description() << std::endl;
      print_exits();
      if(Game::player()->location()->items().size() > 0) {
        std::cout << std::endl << "You can see the following item" << (Game::player()->location()->items().size() > 1 ? "s" : "") << ": " << std::endl;
        print_items(Game::player()->location()->items());
      }
      if(Game::player()->location()->characters().size() > 1) {
        std::cout << std::endl << "You can see the following person" << (Game::player()->location()->characters().size() > 2 ? "s" : "") << ": " << std::endl;
        print_characters(npcs());
      }
		} else {
			std::cout << "You can't go that direction." << std::endl;
		}
	}

	static void cmd_pick_up(ParseData &d) {
		if(d.line.empty()) return print("Pick up what?!");
		Container * c= nullptr;
		Item * i = find_item_and_container(Game::player()->location()->all_items(),d.line, &c);

		if(i != nullptr && c == nullptr) {
				Game::player()->pick_up(i);
		} else if(i != nullptr) {
			Game::player()->take(dynamic_cast<Keepable*>(i), c);
		} else {
			std::cout << "You try to find some " << d.line << " to pick up, but you don't find any." << std::endl;
		}
	}

	static void cmd_take_from(ParseData &d) {
		if(d.line.empty()) return print("WHAT?!");
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
		if(d.line.empty()) return print("Open what?!");
		Item * i = detect_item(Game::player()->location()->items(), d.line);
		Container * c = dynamic_cast<Container*>(i);
		if(c != nullptr) {
			if(c->open(Game::player())) {
				std::cout << "You open " << c->name() << ". Inside you see: " << std::endl;
				print_items(c->content());
			}
		} else {
			std::cout << "You can't find any " << d.line << " to open." << std::endl;
		}
	}

	static void cmd_close(ParseData &d) {
		if(d.line.empty()) return print("Close what?!");
		Item * i = detect_item(Game::player()->location()->items(), d.line);
		Container * c = dynamic_cast<Container*>(i);
		if(c != nullptr) {
			if(c->close(Game::player())) std::cout << "You close " << c->name() << "." << std::endl;
		} else {
			std::cout << "You can't find any " << d.line << " to open." << std::endl;
		}
	}

	static void cmd_drop(ParseData &d) {
		if(d.line.empty()) return print("Drop what?!");
		Keepable * k = dynamic_cast<Keepable*>(detect_item(Game::player()->inventory(), d.line));
		if(k != nullptr) {
			Game::player()->drop(k);
		} else {
			std::cout << "Well, if you want to drop " << d.line << " you will have to pick it up first." << std::endl;
		}
	}

	static void cmd_equip(ParseData &d) {
		if(d.line.empty()) return print("Equip what?!");
		Container * c = nullptr;
		Item * i = find_item_and_container(accessible_items(false), d.line, &c);
		if(i != nullptr) {
			Equipment * e = dynamic_cast<Equipment*>(i);
			if(e != nullptr) {
				Human::slot_t slot = Equipment::default_slot(e->type());

				if(d.line.find("right") != d.line.npos) slot = Human::RIGHT_HAND;
				else if(d.line.find("left") != d.line.npos) slot = Human::LEFT_HAND;

				Game::player()->do_action(5);

				if(c != nullptr) c->take(e);
				else if(Game::player()->inventory().count(e) == 1) { }
				else if(!Game::player()->location()->pick_up(Game::player(), i)) return;

				if(Game::player()->equip(e, slot)) {
					std::cout << "You equipped " << e->name() << " in " << Human::slot_names[slot] << "." << std::endl;
				} else {
					Game::player()->location()->drop(Game::player(), e, true);
				}
			} else {
				std::cout << "You try to equip " << i->name() << " a couple of times, but fail misserably." << std::endl;
			}
		} else {
			std::cout << "For that to be possible, you first must find a " << d.line << "."  << std::endl;
		}
	}

	static void cmd_unequip(ParseData &d) {
		if(d.line.empty()) return print("Unequip what?!");
		std::set<Item*> items;
		for(int i=0; i<Human::NUM_SLOTS; ++i) {
			Human::slot_t slot = static_cast<Human::slot_t>(i);
			Equipment * e = Game::player()->equipment(slot);
			if(e != nullptr) items.insert(e);
		}
		Item * item = detect_item(items, d.line);
		if(item == nullptr) {
			std::cout << "No " << d.line << " equipped." << std::endl;
			return;
		}
		for(int i=0; i<Human::NUM_SLOTS; ++i) {
			Human::slot_t slot = static_cast<Human::slot_t>(i);
			Equipment * e = Game::player()->equipment(slot);
			if(e == item) {
				std::cout << "Unequipping " << Game::player()->equipment(slot)->name() << " from " << Human::slot_names[i] << "." << std::endl;
				Game::player()->unequip(slot);
				return;
			}
		}
	}

	static void cmd_use(ParseData &d) {
		if(d.line.empty()) return print("Use what?!");
		Item * i = detect_item(accessible_items(), d.line);
		if(i != nullptr) {
			Game::player()->do_action(5);
			i->use(Game::player());
		} else {
			std::cout << "You are sure that you saw a " << d.line << " to use just a moment ago, but it seems to be totaly gone. Or you know, maybe you should learn to type." << std::endl;
		}
	}

	static void cmd_make(ParseData &d) {
		if(system("make")) { }
		Game::singleton->stop();
	}

	static void cmd_set_default_attack(ParseData &d) {
		int points = find_number(d.line);
		if(points == INT_MIN) {
			std::cout << "Missing argument: Number of points for default attack" << std::endl;
			return;
		}
		default_attack = points;
		std::cout << "Default attack set to " << points << std::endl;
	}

	static void cmd_set_default_block(ParseData &d) {
		int points = find_number(d.line);
		if(points == INT_MIN) {
			std::cout << "Missing argument: Number of points for default block" << std::endl;
			return;
		}
		default_block = points;
		std::cout << "Default block set to " << points << std::endl;
	}

	static void cmd_attack(Human::slot_t slot, ParseData &d) {
		int points = find_number(d.line);
		if(points == INT_MIN) points = default_attack;
		if(points == INT_MIN) {
			std::cout << "You must specify the number of action points to use on the attack. (Set default attack with 'defaultattack [number]')." << std::endl;
			return;
		}
		if(points < 0) {
			std::cout << "Number of points to attack for can't be negative." << std::endl;
			return;
		}

		Character * character = detect_character(npcs(), d.line);
		if(character == nullptr) character = Game::player()->in_fight_with();
		if(character == nullptr) {
			std::cout << "You must specify a character to attack if you are not currently in a fight." << std::endl;
			return;
		}
		Game::player()->attack(character, points, slot);
	}

	static void cmd_block(int slot, ParseData &d) {
		int points = find_number(d.line);
		if(points == INT_MIN) points = default_block;
		if(points == INT_MIN) {
			std::cout << "You must specify the number of action points to for the block. (Set default block with 'defaultblock [number]')." << std::endl;
			return;
		}
		if(points < 0) {
			std::cout << "Number of points for block for can't be negative." << std::endl;
			return;
		}
		if(slot == -1) {
			if(Game::player()->equipment(Human::LEFT_HAND) != nullptr && Game::player()->remaining_actions(Human::LEFT_HAND)) {
				slot = Human::LEFT_HAND;
			} else {
				slot = Human::RIGHT_HAND;
			}
		}
		player_block_ret_t *ret = (player_block_ret_t*) d.user_data;
		ret->first = slot;
		ret->second = points;
	}

	static void cmd_no_block(ParseData &d) {
		player_block_ret_t *ret = (player_block_ret_t*) d.user_data;
		if(d.line.empty()) {
			std::cout << "You decide not to block." << std::endl;
			ret->first = -1;
			ret->second = 0;
		} else {
			ret->first = -2;
		}
	}

	static void cmd_life(ParseData &d) {
		std::cout << "You have " << Game::player()->life() << " / " << Game::player()->max_life() << "hp." << std::endl;
	}

	/* Parse trees */

	ParseNode Input::parse_trees[Input::NUM_PARSE_TREES] = {
		ParseNode("", nullptr, {
			ParseNode("inventory", cmd_inventory, { }),
			ParseNode("show attributes", cmd_attributes, { }),
      ParseNode("stats", cmd_attributes, { }),
			ParseNode("equipment", cmd_equipment, { }),
			ParseNode("help", cmd_help_default, { }),
			ParseNode("look around", cmd_look_around, { }),
			ParseNode("describe", cmd_inspect, { }),
			ParseNode("inspect", cmd_inspect, { }),
			ParseNode("look at", cmd_inspect, { }),
			ParseNode("talk to", cmd_talk_to, { }),
			ParseNode("open", cmd_open, { }),
			ParseNode("close", cmd_close, { }),
			ParseNode("drop", cmd_drop, { }),
			ParseNode("defaultattack", cmd_set_default_attack, { } ),
			ParseNode("defaultblock", cmd_set_default_block, { } ),
			ParseNode("attack", nullptr, {
				ParseNode("right", std::bind(cmd_attack, Human::RIGHT_HAND, _1), {}),
				ParseNode("left", std::bind(cmd_attack, Human::LEFT_HAND, _1), {}),
				ParseNode("", std::bind(cmd_attack, Human::RIGHT_HAND, _1), {}),
			}),
			ParseNode("next", [](ParseData &d) { Game::player()->next_turn(); }, { }),
			ParseNode("take", nullptr, {
				ParseNode("from", cmd_take_from, {}),
				ParseNode("", cmd_pick_up, {}),
			}),
			ParseNode("pick up", nullptr, {
				ParseNode("from", cmd_take_from, {}),
				ParseNode("", cmd_pick_up, {}),
			}),
			ParseNode("unequip", cmd_unequip, { }),
			ParseNode("equip", cmd_equip, { }),
			ParseNode("life", cmd_life, { }),
			ParseNode("use", cmd_use, { }),
			ParseNode("go", cmd_go, { }),
      ParseNode("backpack", cmd_inventory, { }),
			ParseNode("make", cmd_make, { }),
		}),
		ParseNode("", nullptr, {
			ParseNode("block", nullptr, {
				ParseNode("right", std::bind(cmd_block, Human::RIGHT_HAND, _1), {}),
				ParseNode("left", std::bind(cmd_block, Human::LEFT_HAND, _1), {}),
				ParseNode("", std::bind(cmd_block, -1 , _1), {}),
			}),
			ParseNode("defaultblock", cmd_set_default_block, { } ),
			ParseNode("", cmd_no_block, {}),
		})
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
			names.clear();
			lcased = Game::lowercase(std::string(text));
			last_index = 0;
			len = strlen(text);
			for(const auto & exit : Game::player()->location()->exits()) {
				names.push_back(Game::lowercase(exit.first));
			}
			for(Item * i : accessible_items()) {
				names.push_back(Game::lowercase(i->raw_name()));
			}
			for(Character * c : npcs()) {
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
		} else if(active_parse_tree == Input::DEFAULT) {
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
		rl_completion_entry_function = cmd_completion;
	}

	void Input::cleanup() {

	}

	void Input::read(Input::parse_tree_t tree, const char * prompt, void * user_data) {
		char * line = nullptr;
		bool res;
		active_parse_tree = tree;
		do {
			std::string cmd;
			if(Game::player()->state() != Character::IN_FIGHT && !redo.empty()) {
				cmd = redo;
			} else {
				line = readline(prompt);

				if(line == nullptr) {
					Game::singleton->stop();
					free(line);
					return;
				}
				if(strlen(line) > 0) add_history(line);

				cmd = std::string(line);
				free(line);
			}

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

			try {
				res = ParseNode::parse(parse_trees[tree], cmd, user_data);
				if(!res && cmd.length() != 0) printf("Unknown command %s.\n", cmd.c_str());
				redo = "";
			} catch (const char * err) {
				std::cout << err;
				if(Game::player()->state() != Character::IN_FIGHT) {
					std::cout << std::endl;
					Game::player()->next_turn();
					if(redo.empty()) redo = cmd;
					else redo = "";
				} else {
					std::cout << " Type next to end your turn." << std::endl;
				}
			}
		} while(res == false);
	}

}
