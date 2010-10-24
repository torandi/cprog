#include "gregorian.h"
#include "kattistime.h"

namespace lab2 {
	
	Gregorian::Gregorian() {
		time_t currenttime;
		k_time(&currenttime);

		int days_since_unix = (int) currenttime / (60 * 60 * 24);
		
		_mod_julian_day = days_since_unix + DAYS_BETWEEN_UNIX_AND_MOD_JULIAN;		
	}

	Gregorian::(int year, int month, int day) {
		int a = (14-month)/2;
		int y = year + 4800 - a;
		int m = month + 12*a - 3;

		mod_julian_day = day + (153*m + 2)/5 + 365*y + (y/4) - (y/100) + (y/400) - 32045;
	}


	Gregorian::weekday() const {
		// The constant -5 is due to _mod_julian_day counting from a wednesday.
		return ((_mod_julian_day-5)%7 +1);
	}

	Gregorian::days_this_month() const {
		
	}

	Gregorian::&operator+=(const int n) {
		_mod_julian_day += n;
	}

	Gregorian::&operator-=(const int n) {
		_mod_julian_day -= n;
	}
};
