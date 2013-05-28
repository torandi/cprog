#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "game.hpp"

namespace game {
	class Area;
	class Item;

	class Character {
	public:
		enum state_t {
			IDLE,
			IN_FIGHT,
			IN_DIALOG,
			DEAD
		};

		virtual const int life() const;
		const int max_life() const;

		virtual void regain_life(int life);

		virtual const std::string &faction() const;
		virtual const std::string &name() const;
		virtual const std::string &description() const;
		state_t state() const;

		const Area* location() const;

		virtual const int initiative() const;

		virtual void new_turn();
		virtual void action() = 0;

		virtual void attack(Character * character) = 0;
		virtual void incoming_attack(Character * character, int damage) = 0;
		virtual void go(const std::string &direction);

		virtual int attribute(const std::string &attr) const;

		virtual void do_action(int cost);
		virtual Game::try_result_t try_do_action(int cost);

		virtual int armor_protection() const;

		virtual void pick_up(Item * item);

		virtual ~Character();
	protected:
		Character(const std::string &name, const std::string &faction, const std::string &description, int life, std::map<std::string, int> attributes, Area * location);

		int initiative_roll() const;
		virtual void store(Item * item) = 0;

		const int m_max_life;
		const std::string m_name, m_faction, m_description;

		std::map<std::string, int> m_attributes;

		int m_action_mod = 0;
		int m_action_points = 0;

		int m_life;
		state_t m_state = IDLE;


		Area * m_location;

		Character * m_in_fight = nullptr;

	};
};

#endif
