ACLOCAL_AMFLAGS = -I m4
AM_DEFAULT_SOURCE_EXT = .cpp

AM_CXXFLAGS = -Wall -Woverloaded-virtual -Wvla -Wconversion ${yaml_CFLAGS} -I${top_srcdir}/src

bin_PROGRAMS = textrpg

textrpg_SOURCES = \
	src/area.cpp src/area.hpp \
	src/game.cpp src/game.hpp \
	src/item.cpp src/item.hpp \
	src/health_potion.cpp src/health_potion.hpp \
	src/character.cpp src/character.hpp \
	src/config.cpp src/config.hpp \
	src/logging.cpp src/logging.hpp \
	src/keepable.cpp src/keepable.hpp \
	src/container.cpp src/container.hpp \
	src/human.cpp src/human.hpp \
	src/equipment.cpp src/equipment.hpp \
	src/world_parser.cpp src/world_parser.hpp \
	src/parse_node.cpp src/parse_node.hpp \
	src/player.cpp src/player.hpp \
	src/monster.cpp src/monster.hpp \
	src/input.cpp src/input.hpp \
	src/string_utils.cpp src/string_utils.hpp \
	src/unique_item.cpp src/unique_item.hpp \
	src/requiring_character.cpp src/requiring_character.hpp \
	src/darkwood.cpp src/darkwood.hpp \
	src/ancient_stone.cpp src/ancient_stone.hpp \
	src/main.cpp \
	src/color.hpp

textrpg_LDADD = -lreadline ${yaml_LIBS}
