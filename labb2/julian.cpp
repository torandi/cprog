#include "julian.h"
#include "kattistime.h"

namespace lab2 {

	Julian::Julian() {
		time_t currenttime;
		k_time(&currenttime);

		int days_since_unix = (int) currenttime / (60*60*24);
		_mod_julian_day  = days_since_unix + DAYS_BETWEEN_UNIX_AND_MOD_JULIAN;
	}

	Julian::Julian(int year, int month, int day) {
		_mod_julian_day = 367*year + (7*(y+((m+9)/2)))/4 + 285*month / 9 + day;
	}

	
	const int Julian::week_day() const {
				
	}

	const int days_this_month() const {

	}
}
