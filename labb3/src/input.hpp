#ifndef INPUT_HPP
#define INPUT_HPP

#include "parse_node.hpp"

namespace game {
	class Input {
		public:

			enum parse_tree_t {
				DEFAULT=0,
				DEFEND,
				NUM_PARSE_TREES
			};

			static void init();
			static void cleanup();

			static void read(parse_tree_t tree, const char * prompt = ">> ", void * user_data = nullptr);

			static ParseNode parse_trees[];

	};
};

#endif
