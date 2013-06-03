#include "ancient_stone.hpp"
#include "character.hpp"
#include "game.hpp"
#include "unique_item.hpp"
#include <sstream>

namespace game {
  AncientStone::AncientStone(const std::string &name, const std::string &description, const std::string &spawn_enemy)
    : Item(name, description)
    , m_spawn_enemy(spawn_enemy)
    , m_spawned_enemy(false)
    , m_stone_count(0) {
  }

	bool AncientStone::use(Character * character) const {
    if(!m_spawned_enemy) {
      for(int i=0; i<3; ++i) {
        UniqueItem * item = nullptr;
        if(!m_placed_stones[i] && (item = character->have_unique(m_stone_ids[i]))) {
          ++m_stone_count;
          m_placed_stones[i] = true;
          character->delete_item(item);
          Game::out(character->location()) << "You place the " << item->name() << " in one of the slots on the standing stone." << std::endl;
          delete item;
        }
      }
      Game::out(character->location()) << "When you place the last magic stone on the standing stone the ground opens beyond it and a huge monster appears." << std::endl;
      Character * monster = Character::from_id(m_spawn_enemy, character->location());
      Game::singleton->add_character(monster);
    }
    return false;
	}

  AncientStone * AncientStone::from_config(const ConfigNode * node) {
    return new AncientStone(
        (*node)["/name"].parse_string(),
        (*node)["/description"].parse_string(),
        (*node)["/enemy"].parse_string()
        );
  }

	std::string AncientStone::description() const {
		std::stringstream str;
		str << Item::description();
    if(m_stone_count > 0) {
      str << ". You have placed " << m_stone_count << " magic stone" << (m_stone_count > 1 ? "s" : "") << " in the slots on the stone.";
    }
		return str.str();
	}

  const std::string AncientStone::m_stone_ids[3] = {
    "purple_magic_stone",
    "dark_red_magic_stone",
    "marine_magic_stone",
  };
};
