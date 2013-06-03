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

  std::vector< std::vector<WorldParser::item_prefix_t> > WorldParser::prefix_table[Equipment::NUM_TYPES];
  std::vector<std::discrete_distribution<int> > WorldParser::prefix_distribution_table[Equipment::NUM_TYPES];
  const ConfigNode * WorldParser::current_prefix_probability = nullptr;

	void WorldParser::parse(Game * game) {
    Config items_config = Config::from_filename("game/items.yaml");
    for(auto eq_type : items_config["/equipment_prefixes"].map()) {
      /* Equipment type */
      Equipment::type_t type = Equipment::map_type(eq_type.first);
      std::vector<item_prefix_t> prefixes;
      std::vector<int> pfx_prob;

      for(const ConfigNode * group_node : eq_type.second->list()) {
        /* Prefix group */
        prefixes.clear();
        pfx_prob.clear();
        int num_items = static_cast<int>(group_node->list().size());
        int index = 0;

        item_prefix_t::type_t pfx_type;
        if(group_node->tag() == "!prefix") {
          pfx_type = item_prefix_t::PREFIX;
        } else if(group_node->tag() == "!suffix") {
          pfx_type = item_prefix_t::SUFFIX;
        } else {
          std::cout << "Invalid tag " << group_node->tag() << " for prefix group" << std::endl;;
          abort();
        }

        for(const ConfigNode * prefix_node : group_node->list()) {
          /* Prefix */

          item_prefix_t prefix = { (*prefix_node)["/name"].parse_string(), {}, pfx_type};
          for(auto attr : (*prefix_node)["/attributes"].map()) {
            prefix.attributes[attr.first] = attr.second->parse_int();
          }
          prefixes.push_back(prefix);
          if(pfx_type == item_prefix_t::PREFIX) {
            pfx_prob.push_back(num_items - index++);
          } else {
            pfx_prob.push_back(1); /* Affixes have linear probability */
          }
        }
        prefix_table[type].push_back(prefixes);
        prefix_distribution_table[type].push_back(std::discrete_distribution<int>(pfx_prob.begin(), pfx_prob.end()));
      }
    }

		parse_areas(game);
		parse_items(game);

		Config player_config = Config::from_filename("game/player.yaml");

		Area * player_location = game->area(player_config["/location"].parse_string());
		game->m_player = new Player(Character::parse_attributes(&player_config["/attributes"]), player_location);
		Human::parse_equipment(game->m_player, player_config.find("/equipment", false));
		game->add_character(game->m_player);
	}

	void WorldParser::parse_areas(Game * game) {
		Config areas_config = Config::from_filename("game/areas.yaml");
		std::map<std::string,Area*> &areas = game->areas;

		for(const ConfigNode * node : areas_config.root().list()) {
			Area * area = Area::from_config(node);
			areas[(*node)["/id"].parse_string()] = area;

      const ConfigNode *nri = node->find("/no_random_items");
      if(nri !=nullptr && nri->parse_bool() == true) {
        area->m_no_random_items = true;
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
          game->add_character(Character::from_node(npc, area));
        }
      }
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

	/*
	 * Parse items that are randomly placed
	 */
	void WorldParser::parse_items(Game * game) {
		Config items_config = Config::from_filename("game/items.yaml");
		std::map<std::string,Area*> &areas = game->areas;

		std::vector<const ConfigNode*> containers = items_config["/containers"].list();
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

      if(!it.second->m_no_random_items) {
        int item_count = items_config["/item_count"].parse_int();
        if(item_count > 0) {
          Container * container = Container::from_config(containers[container_select(generator)]);
          for(Keepable * item : random_items(items_config["/random_items"].list(), item_count)) {
            if(!container->put(item)) delete item;
          }

          area_items.insert(container);
        }
      }
		}
	}

  std::vector<Keepable*> WorldParser::random_items(const std::vector<const ConfigNode*> &items, int count) {
    std::uniform_int_distribution<int> item_select(0, static_cast<int>(items.size()) - 1);

    std::vector<Keepable*> result;
    for(int i=0; i < count; ++i) {
      Item * item = Item::from_node(items[item_select(generator)]);
      Keepable * keepable = dynamic_cast<Keepable*>(item);
      if(keepable == nullptr) delete item;
      else result.push_back(keepable);
    }
    return result;
  }

  std::vector<WorldParser::item_prefix_t> WorldParser::generate_prefixes(Equipment::type_t type) {
    std::vector<item_prefix_t> ret;

    if(prefix_table[type].size() == 0) return ret;

    Config item_config = Config::from_filename("game/items.yaml");
    const ConfigNode * prefix_probability = current_prefix_probability;

    if(prefix_probability == nullptr) {
      prefix_probability = &item_config["/default_prefix_probability"];
    }

    std::vector<unsigned int> groups;
    for(unsigned int i=0; i< prefix_table[type].size(); ++i) {
      groups.push_back(i);
    }


    std::vector<int> prob_distribution;
    unsigned int c = 0;
    for(const ConfigNode * node : prefix_probability->list()) {
      if(c <= groups.size()) {
        prob_distribution.push_back(node->parse_int());
      } else {
        prob_distribution[groups.size()-1] += node->parse_int();
      }
      ++c;
    }

    std::discrete_distribution<int> num_prefixes_distr(prob_distribution.begin(), prob_distribution.end());
    int num_prefixes = num_prefixes_distr(generator);
    for(int i=0; i < num_prefixes; ++i) {
      std::uniform_int_distribution<unsigned int> rnd_group(0, static_cast<unsigned int>(groups.size()) - 1);
      int g_index = rnd_group(generator);
      auto g_it = groups.begin() + g_index;
      int g = *g_it;
      groups.erase(g_it);

      int pfx = prefix_distribution_table[type][g](generator);
      ret.push_back(prefix_table[type][g][pfx]);
    }
    return ret;
  }

}
