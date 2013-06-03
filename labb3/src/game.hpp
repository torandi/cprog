#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>
#include <random>
#include <iostream>

namespace game {
	class Character;
	class Area;
	class WorldParser;
  class Player;
  class Keepable;

	class Game {
		public:
			static Game * singleton;

			Game();
			void init();
			~Game();

			enum dice_t {
				T5=0,
				T6,
				T10,
				T20
			};

			enum try_result_t {
				PERFECT=0,
				SUCCESS=1,
				FAIL=2,
				FATAL=3
			};

			static int roll_dice(dice_t dice, int op=-1);
			static try_result_t try_action(int points);

			/* Return output stream (none/stdout) to use for a given area */
			std::ostream &output(const Area* a1, const Area* a2=nullptr);
			static std::ostream &out(const Area* a1, const Area* a2=nullptr); /* alias for Game::singleton->output */
      static Player * player();

      static std::string lowercase(const std::string &str);

      Area * area(const std::string &name);
      Keepable * item(const std::string &name);

      void add_character(Character * character);
			void character_dies(Character * character);

      void start_simulation();
      void stop();

			void set_prefix_mode(bool new_mode) { m_prefixes_enabled = new_mode; };
			bool get_prefix_mode() { return m_prefixes_enabled; };

			bool run() const { return m_run; };
		private:
			static std::default_random_engine generator;
			static std::vector<std::uniform_int_distribution<int> > dices;

			std::map<std::string,Area*> areas;
      std::map<std::string,Keepable*> items;
			std::vector<Character*> characters;
      Player * m_player;

      bool m_run = true;
			bool m_won = false;
			bool m_prefixes_enabled = true;
			std::string game_name, intro, end_text, author;
			Character * final_monster = nullptr;
			std::string final_monster_name;

			friend class WorldParser;

	};
};

#endif
