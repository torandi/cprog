#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>
#include <random>

namespace game {
	class Game {
		public:
			static Game * singleton;

			Game();
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

		private:
			static std::default_random_engine generator;
			static std::vector<std::uniform_int_distribution<int> > dices;

	};
};

#endif
