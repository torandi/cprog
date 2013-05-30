#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "world_parser.hpp"
#include "game.hpp"
#include "character.hpp"
#include "area.hpp"
#include "logging.hpp"
#include "health_potion.hpp"
#include "item.hpp"
#include "container.hpp"
#include "keepable.hpp"
#include "player.hpp"

#include <chrono>


namespace game {
	std::default_random_engine WorldParser::generator(std::chrono::system_clock::now().time_since_epoch().count());

	void WorldParser::parse(Game * game) {
		parse_areas(game);
		parse_items(game);

		Config game_config = Config::from_filename("game/game.yaml");

		Area * player_location = game->area(game_config["/player/location"].parse_string());
		game->m_player = new Player(Character::parse_attributes(&game_config["/player/attributes"]), player_location);
		Human::parse_equipment(game->m_player, game_config.find("/player/equipment", false));
	}

	void WorldParser::parse_areas(Game * game) {
		Config areas_config = Config::from_filename("game/areas.yaml");
		std::map<std::string,Area*> &areas = game->areas;

		for(const ConfigNode * node : areas_config.root().list()) {
			Area * area = Area::from_config(node);
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

			const ConfigNode * items = node->find("/items");
			if(items) {
				for(const ConfigNode * i : items->list()) {
					area->m_items.insert(Item::from_node(i));
				}
			}

			const ConfigNode * npcs = node->find("/npcs");
			if(npcs) {
				for(const ConfigNode * npc : npcs->list()) {
					area->m_characters.insert(Character::from_node(npc, area));
				}
			}
		}
	}

	/*
	 * Parse items that are randomly placed
	 */
	void WorldParser::parse_items(Game * game) {
		Config items_config = Config::from_filename("game/items.yaml");
		std::map<std::string,Area*> &areas = game->areas;


		std::vector<const ConfigNode*> items = items_config["/random_items"].list();
		std::vector<const ConfigNode*> containers = items_config["/containers"].list();
		std::uniform_int_distribution<int> item_select(0, static_cast<int>(items.size()) - 1);
		std::uniform_int_distribution<int> container_select(0, static_cast<int>(containers.size()) - 1);


		for(auto it : areas) {
			std::set<Item*> &area_items = it.second->m_items;

			int health_potions = items_config["/health_potion/count"].parse_int();
			if(health_potions > 0) {
				for(int i=0; i<health_potions; ++i) {
					HealthPotion * hp = HealthPotion::from_config(&(items_config["/health_potion"]));
					area_items.insert(hp);
				}
			}

			int item_count = items_config["/item_count"].parse_int();
			if(item_count > 0) {
				Container * container = Container::from_config(containers[container_select(generator)]);
				for(int i=0; i < item_count; ++i) {
					Keepable * item = dynamic_cast<Keepable*>(Item::from_node(items[item_select(generator)]));
					if(!container->put(item)) delete item;
				}
				area_items.insert(container);
			}
		}
	}

}
