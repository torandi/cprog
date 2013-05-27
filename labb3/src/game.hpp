#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>
#include <random>

namespace game {
	class Game {
		public:
			Game();
			~Game();

			enum dice_t {
				T5=0,
				T6,
				T10,
				T20
			};

			static int roll_dice(dice_t dice, int op=-1);

		private:
			static std::default_random_engine generator;
			static std::vector<std::uniform_int_distribution<int> > dices;

	};
};

#endif
