#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "world_parser.hpp"
#include "game.hpp"
#include "character.hpp"
#include "area.hpp"
#include "logging.hpp"

namespace game {
	void WorldParser::parse(Game * game) {
		/* Load world */
		Config game_config = Config::from_filename("game/game.yaml");
		//Config item_config = Config::from_filename("game/items.yaml");
		//
		parse_areas(game);
	}

	void WorldParser::parse_areas(Game * game) {
		Config areas_config = Config::from_filename("game/areas.yaml");
		std::map<std::string,Area*> &areas = game->areas;

		for(const ConfigNode * node : areas_config.root().list()) {
			Area * area = nullptr;
			if(node->tag() == "!area") {
				area = Area::from_config(node);
			} else {
				node->print();
				Logging::fatal("Invalid or missing tag for area");
			}
			areas[(*node)["/id"].parse_string()] = area;
		}


		//Fix area exits
		for(const ConfigNode * node : areas_config.root().list()) {
			Area * area = areas.find((*node)["/id"].parse_string())->second;
			const ConfigNode * exits_node = node->find("/exits");
			if(exits_node) {
				std::map<std::string, Area*> exits;
				for(auto &exit : exits_node->map()) {
					auto e_area = areas.find(exit.second->parse_string());
					if(e_area == areas.end()) {
						Logging::fatal("Unknown area id %s for exit %s from area %s\n", exit.second->parse_string().c_str(), exit.first.c_str(), area->name().c_str());
					}
					exits[exit.first] = e_area->second;
				}
				area->set_exits(exits);
			}
		}
	}

}
