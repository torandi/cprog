#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "requiring_character.hpp"
#include "keepable.hpp"
#include "game.hpp"
#include "area.hpp"
#include "item.hpp"
#include "unique_item.hpp"
#include "logging.hpp"

namespace game {
	void RequiringCharacter::incoming_attack(Character * character, int damage, bool critical_hit) {
		Game::out(location()) << name() << " dodges " << character->genitive() << " attack." << std::endl;
	}

	RequiringCharacter * RequiringCharacter::from_config(const ConfigNode * node, Area * location) {

		std::string name = (*node)["/name"].parse_string();
		std::string description = (*node)["/description"].parse_string();
		std::string required_item = (*node)["/required_item"].parse_string();
    WorldParser::current_prefix_probability = node->find("/prefix_probability");
		Keepable * item = dynamic_cast<Keepable*>(Item::from_node(node->find("/give_item", true)));
    WorldParser::current_prefix_probability = nullptr;
		if(item == nullptr) Logging::fatal("Requiring Character %s don't have a correct /give_item\n", name.c_str());
		RequiringCharacter * rc = new RequiringCharacter(name, description, location, required_item, item);

		const ConfigNode * dialog = node->find("/dialog", true);
		for(const ConfigNode * d : dialog->list()) {
			rc->m_require_dialog.push_back(d->parse_string());
		}

		dialog = node->find("/give_dialog", false);
		if(dialog) {
			rc->m_give_dialog = dialog->parse_string();
		} else {
			rc->m_give_dialog = "Here, take this!";
		}

		dialog = node->find("/after_given_dialog", false);
		if(dialog) {
			rc->m_no_more_dialog = dialog->parse_string();
		} else {
			rc->m_no_more_dialog = "I have nothing more to offer you!";
		}

		return rc;
	}

	RequiringCharacter::RequiringCharacter(const std::string &name, const std::string &description,
			Area * location, const std::string &required_item, Keepable * give_item)
		: Character(name, description, Character::INVINCIBLE, { }, location)
		, m_required_item(required_item)
		, m_give_item(give_item) {}

	bool RequiringCharacter::talk_to(Character  * character) {
		Game::out(location());
		UniqueItem * item = character->have_unique(m_required_item);
		if(m_given_item) {
			Game::out(location())  << m_no_more_dialog << std::endl;
		} else if(item == nullptr) {
			Game::out(location())  << m_require_dialog[m_next_dialog]  << std::endl;
			m_next_dialog = (m_next_dialog + 1) % static_cast<int>(m_require_dialog.size());
		} else {
			Game::out(location()) << m_give_dialog << std::endl;
			Game::out(location()) << name() << " gave you " << m_give_item->name() << std::endl;
			character->delete_item(item);
			character->store(m_give_item);
			m_given_item = true;
		}

		return true;
	}
}
