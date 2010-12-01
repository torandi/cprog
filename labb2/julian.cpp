#include <cmath>
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
		set_mjd_from_ymd(ymd_t(year,month,day));
	}

	Julian::Julian(const Date &date) {
		_mod_julian_day = date.mod_julian_day();
	}

	Julian::Julian(Date * &date) {
		_mod_julian_day = date->mod_julian_day();
	}

	void Julian::set_mjd_from_ymd(const ymd_t &ymd) {
		_mod_julian_day = 367*ymd.y + floor((7*(ymd.y+floor((ymd.m+9)/12)))/4) + 
			floor(285*ymd.m / 9) + ymd.d - 730530;
	}

	const int Julian::week_day() const {
		return (_mod_julian_day % 7)+4;			
	}

	const int Julian::days_this_month() const {
		return 0;
	}

	const float Julian::mjd_to_jd() const {
		return _mod_julian_day + DAYS_BETWEEN_MJD_AND_JD;
	}

	ymd_t Julian::mjd_to_ymd() const {
		int x1 = (int) mjd_to_jd() - 1721117.5;
		int y = (int) floor((4*x1 + 3)/1461);
		int x0 = (int) x1 - floor(1461*y/4);
		int m = (int) floor((5*x0 + 461)/153);
		int d = (int) x0 - floor((153*m - 457)/5) + 1;

		if (m>12)
		{
			m -= 12;
			++y;
		}

		return ymd_t(y,m,d);
	}

	const bool Julian::is_leap_year(int year) const {
		return !(year%4);
	}
}
