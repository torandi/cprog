#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

inline std::ostream &black (std::ostream &s) {
	s << "\033[0;30m";
	return s;
}

inline std::ostream &darkgrey (std::ostream &s) {
	s << "\033[1;30m";
	return s;
}

inline std::ostream &blue (std::ostream &s) {
	s << "\033[0;34m";
	return s;
}

inline std::ostream &lightblue (std::ostream &s) {
	s << "\033[1;34m";
	return s;
}

inline std::ostream &green (std::ostream &s) {
	s << "\033[0;32m";
	return s;
}

inline std::ostream &lightgreen (std::ostream &s) {
	s << "\033[1;32m";
	return s;
}

inline std::ostream &cyan (std::ostream &s) {
	s << "\033[0;36m";
	return s;
}

inline std::ostream &lightcyan (std::ostream &s) {
	s << "\033[1;36m";
	return s;
}

inline std::ostream &red (std::ostream &s) {
	s << "\033[0;31m";
	return s;
}

inline std::ostream &lightred (std::ostream &s) {
	s << "\033[1;31m";
	return s;
}

inline std::ostream &purple (std::ostream &s) {
	s << "\033[0;35m";
	return s;
}

inline std::ostream &lightpurple (std::ostream &s) {
	s << "\033[1;35m";
	return s;
}

inline std::ostream &brown (std::ostream &s) {
	s << "\033[0;33m";
	return s;
}

inline std::ostream &yellow (std::ostream &s) {
	s << "\033[1;33m";
	return s;
}

inline std::ostream &normal (std::ostream &s) {
	s << "\033[0m";
	return s;
}

inline std::ostream &show_colors(std::ostream &s) {
	s << blue << "blue " << lightblue << " lightblue " << lightgreen << " lightgreen " << green << " green " << cyan << " cyan " << lightcyan << " lightcyan " << purple << " purple " << lightpurple << " lightpurple " << red << " red " << lightred << " lightred " << brown << " brown " << yellow << " yellow " << normal << std::endl;
	return s;
}



#endif
