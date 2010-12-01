#include "date.h"
#include <cstdio>

namespace lab2 {
	Date& Date::operator=(const Date &date) {
		_mod_julian_day=date._mod_julian_day;
		return *this;
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

std::ostream & operator<<(std::ostream & os, const lab2::Date &date) {
	//Fix padding:
	char buffer[10];
	std::sprintf(buffer,"%04d-%02d-%02d",date.year(),date.month(),date.day());
	os<<buffer;
	return os;
}
