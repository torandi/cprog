#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

namespace game {
	class Game;
	class Area;

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

		virtual const std::string &type() const;
		virtual const std::string &name() const;
		state_t state() const;

		const Area* location() const;

		virtual const int initiative() const;
		virtual void action(Game * game) = 0;
		virtual void fight(Character * character);
		virtual void go(const std::string &direction);
		//virtual void talk_to(Character * character);
		//pick_up
		//drop

		protected:
			Character(const std::string &name, const std::string &type, int life, int initiative, Area * location);
			~Character();

			const int m_max_life;
			const std::string m_name, m_type;

			const int m_initiative;

			int m_life;
			state_t m_state = IDLE;

			Area * m_location;

			Character * m_in_fight = nullptr;

			int initiative_roll() const;

	};
};

#endif
