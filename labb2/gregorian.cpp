#include <cmath>
#include "gregorian.h"
#include "kattistime.h"

namespace lab2 {
	
	Gregorian::Gregorian() {
		time_t currenttime;
		k_time(&currenttime);

		int days_since_unix = (int) currenttime / (60 * 60 * 24);
		
		_mod_julian_day = days_since_unix + DAYS_BETWEEN_UNIX_AND_MOD_JULIAN;		
	}

	Gregorian::Gregorian(const Date &date) {
		_mod_julian_day=date.mod_julian_day();
	}

	Gregorian::Gregorian(int year, int month, int day) {
		ymd_t ymd(year,month,day);
		check_valid_date(ymd);
		set_mjd_from_ymd(ymd);
	}


	const int Gregorian::week_day() const {
		return (_mod_julian_day+2)%7+1;
	}
	
/*
	Gregorian Gregorian::operator++(int i) {
		Gregorian g(*this);
		++(*this);
		return g;
	}	
	*/

	const float Gregorian::mjd_to_jd() const {
		return _mod_julian_day + DAYS_BETWEEN_MJD_AND_JD;
	}


	const bool Gregorian::is_leap_year(int y) const {
		if(y % 4 == 0) {
			if(y % 100 == 0) {
				if(y % 400 == 0) {
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	}

	ymd_t Gregorian::mjd_to_ymd() const {
		float x2 = mjd_to_jd() - 1721119.5;
		float c2 = floor((4*x2+3)/146097); 
		float x1 = x2 - floor(146097*(c2/4));
		float c1 = floor((100*x1 + 99)/36525);
		float x0 = x1 - floor(36525*c1/100);

		int y = 100*c2 + c1;
		int m = floor((5*x0 + 461)/153);
		int d = x0 - floor((153*m -457)/5) + 1;
		if(m>12) {
			m-=12;
			++y;
		}

		return ymd_t(y,m,d);
	}

	void Gregorian::set_mjd_from_ymd(const ymd_t &ymd) {
		float a = floor((14-ymd.m)/12);
		float y = ymd.y + 4800 - a;
		float m = ymd.m + (12*a) - 3;
		
		_mod_julian_day = ymd.d + floor((153*m + 2)/5) + (365*y) + floor(y/4) - floor(y/100) + floor(y/400) - 32045 -0.5- DAYS_BETWEEN_MJD_AND_JD;
	}
}
