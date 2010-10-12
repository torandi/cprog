#include "date.h"

namespace lab2 {
	Date& Date::operator=(const Date &date) {
		_mod_julian_day=date._mod_julian_day;
	}

	bool Date::operator==(const Date &date) const {
		return date._mod_julian_day==_mod_julian_day;
	}

	bool Date::operator!=(const Date &date) const{
		return !(*this==date);
	}


}
