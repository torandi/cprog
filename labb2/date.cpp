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

	bool Date::operator<(const Date &date) const {
		return mod_julian_day() < date.mod_julian_day();
	}

	bool Date::operator>(const Date &date) const {
		return mod_julian_day() > date.mod_julian_day();
	}

	bool Date::operator<=(const Date &date) const {
		return mod_julian_day() <= date.mod_julian_day();
	}

	bool Date::operator>=(const Date &date) const {
		return mod_julian_day() >= date.mod_julian_day();
	}

	Date & Date::operator++() { return *this+=1; };
	Date & Date::operator--() { return *this-=1; };

	Date &Date::operator+=(const int n) {
		_mod_julian_day += n;
		return *this;
	}

	Date &Date::operator-=(const int n) {
		_mod_julian_day -= n;
		return *this;
	}

	int Date::operator-(const Date &date) const {
		return _mod_julian_day-date.mod_julian_day();
	}

}
