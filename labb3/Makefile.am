ACLOCAL_AMFLAGS = -I m4
AM_DEFAULT_SOURCE_EXT = .cpp

AM_CXXFLAGS = -Wall -Woverloaded-virtual -Wvla -Wconversion ${yaml_CFLAGS} -I${top_srcdir}/src

bin_PROGRAMS = cprog10l3

cprog10l3_SOURCES = \
	src/game.cpp src/game.hpp \
	src/character.cpp src/character.hpp \
	src/config.cpp src/config.hpp \
	src/logging.cpp src/logging.hpp \
	src/main.cpp

cprog10l3_LDADD = ${yaml_LIBS}
