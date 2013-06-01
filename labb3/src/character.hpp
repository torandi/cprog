#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "game.hpp"
#include "config.hpp"

namespace game {
	class Area;
	class Item;
	class Keepable;
	class Container;

	class Character {
	public:
		enum state_t {
			IDLE,
			IN_FIGHT,
			DEAD
		};

		enum faction_t {
			CIVILIAN,
			MONSTERS,
			BANDITS,
			UNALIGNED,
			NUM_FACTIONS
		};


		virtual const int life() const;
		const int max_life() const;

		virtual void regain_life(int life);

		virtual faction_t faction() const;
		virtual std::string name() const;
		virtual std::string genitive() const;
		virtual std::string description() const;
		state_t state() const;

		virtual std::map<std::string, int> attributes() const;

		Area* location() const;

		virtual int initiative() const; /* Get's stored initiative */
    virtual void roll_initiative(); /* Reroll initiative */

		virtual void action() = 0;

		virtual void attack(Character * character, int points) = 0;
		virtual void incoming_attack(Character * character, int damage);
		virtual bool go(const std::string &direction);

		virtual int attribute(const std::string &attr) const;
		virtual int extra_damage() const; /* extra damage from strength (and possibly other things) */

		virtual void do_action(int cost);
		virtual Game::try_result_t try_do_action(int cost);

		virtual int armor_protection() const;

		virtual void pick_up(Item * item);
		virtual void use(Item * item);
		virtual void take(Keepable * item, Container * from_container);

		virtual void end_fight(Character * character);

		static Character * from_config(const ConfigNode * node, Area * location);
		static Character * from_node(const ConfigNode * node, Area * location);

		static std::map<std::string, int> parse_attributes(const ConfigNode * node);
		static void parse_inventory(Character * character, const ConfigNode * node);

		virtual std::string verb(const std::string &verb) const;

		Character * in_fight_with() const;

		virtual void init_round();
		virtual ~Character();
	protected:
		Character(const std::string &name, const std::string &description, faction_t faction, std::map<std::string, int> attributes, Area * location);

		virtual void store(Keepable  * item) = 0;
		virtual bool use_action(int hand);

		virtual void die() = 0; //called when the character dies
		virtual void roll_attack(Game::dice_t dice, int points, Character * on_character, int op, int extra, const std::string &weapon_text = "");

		virtual void hurt(int damage); //Apply damage (armor is applied)
		virtual void reduce_armor(int amount) = 0; // Should reduce armor by given amount

		void search_for_enemy();

		const std::string m_name, m_description;
		const faction_t m_faction;

		std::map<std::string, int> m_attributes;

		int m_action_mod = 0;
		int m_action_points = 0;
		int m_remaining_actions[2] = {0, };
    int m_initiative = 0;
		int m_tmp_action_mod = 0;

		int m_life;
		state_t m_state = IDLE;


		Area * m_location;

		Character * m_in_fight = nullptr;
		std::string m_enemy_direction = "";

		/* True indicates agressive */
		static bool faction_standings[Character::NUM_FACTIONS][Character::NUM_FACTIONS];

		static std::map<std::string, std::string> m_third_person_verbs;
	private:
		int m_base_actions[2] = {1, };
		static std::map<std::string, std::function<Character*(const ConfigNode*, Area * location)> > tag_map;

	};
};

#endif
