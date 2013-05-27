#ifndef CHARACTER_HPP
#define CHARACTER_HPP

class Game;

namespace game {
	class Character {
	public:
		enum state_t {
			IDLE,
			IN_FIGHT,
			IN_DIALOG,
			DEAD
		};

		const int life() const;
		const int max_life() const;


		const std::string &type() const;
		const std::string &name() const;
		state_t state() const;

		virtual const int initiative() const;
		virtual void action(Game * game) = 0;
		virtual void fight(Character * character);
		//virtual void talk_to(Character * character);
		//pick_up
		//drop
		//go

		protected:
			Character(const std::string &name, const std::string &type, const std::string &faction, int life, int initiative, int );
			~Character();

			const int m_max_life;
			const std::string name, type, faction;

			const int m_initiative;

			int m_life;
			state_t state = IDLE;

			Character * in_fight = nullptr;


	};
};

#endif
