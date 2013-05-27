#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "game.hpp"

namespace game {
	class Area;

	class Character {
	public:
		enum state_t {
			IDLE,
			IN_FIGHT,
			IN_DIALOG,
			DEAD
		};

		enum equipment_slot_t {
			RIGHT_HAND=0,
			LEFT_HAND,
			ARMOR,
			RING,
			BACKPACK,
			NUM_EQUIPMENT_SLOTS
		};

		virtual const int life() const;
		const int max_life() const;

		virtual const std::string &type() const;
		virtual const std::string &name() const;
		virtual const std::string &description() const;
		state_t state() const;

		const Area* location() const;

		virtual const int initiative() const;

		virtual void new_turn();
		virtual void action() = 0;
		virtual void fight(Character * character);
		virtual void go(const std::string &direction);

		virtual void do_action(int cost);
		virtual Game::try_result_t try_do_action(int cost);

		//pick_up
		//int damage()
		//take_damage()

	protected:
		Character(const std::string &name, const std::string &type, const std::string &description, int life, int initiative, int action_points, Area * location);
		~Character();

		const int m_max_life;
		const std::string m_name, m_type, m_description;

		const int m_initiative, m_base_action_points;
		int m_action_points;

		int m_life;
		state_t m_state = IDLE;

		Area * m_location;

		Character * m_in_fight = nullptr;

		int initiative_roll() const;

	};
};

#endif
