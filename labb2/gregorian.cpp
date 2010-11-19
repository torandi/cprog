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

	Gregorian::Gregorian(int year, int month, int day) {
		ymd_t ymd(year,month,day);
		set_mjd_from_ymd(ymd);
	}


	const int Gregorian::week_day() const {
		// The constant -5 is due to _mod_julian_day counting from a wednesday.
		return (_mod_julian_day%7)+4;
	}
	
	const int Gregorian::days_this_month() const {
		ymd_t ymd = mjd_to_ymd();
		int y = ymd.y;
		
		if((ymd.m == 2) && is_leap_year(y)) {
			return 29;
		} else {
			return days_per_month[ymd.m];
		}
	}
/*
	Gregorian Gregorian::operator++(int i) {
		Gregorian g(*this);
		++(*this);
		return g;
	}	
	*/

	const float Gregorian::mjd_to_jd() const {
		return _mod_julian_day + 2400000.5;
	}

	void Gregorian::add_month(const int months) {
		ymd_t ymd=mjd_to_ymd();
		for(int i=0;i<months;++i) {
			++ymd.m;
			if(ymd.m>12) { 
				ymd.m=1;
				++ymd.y;
			}
			int days;
			if((ymd.m ==2) && is_leap_year(ymd.y))
				days=29;
			else
				days=days_per_month[ymd.m];
			if(ymd.d > days) {
				//Just add 30 days
				_mod_julian_day+=30;
			} else {
				set_mjd_from_ymd(ymd);
			}
		}
	}

	void Gregorian::add_year(const int years) {
		ymd_t ymd=mjd_to_ymd();
		ymd.y+=years;
		if(ymd.m==2 && ymd.d==29 && !is_leap_year(ymd.y))
			ymd.d=28;
		set_mjd_from_ymd(ymd);
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
		int x2 = (int) mjd_to_jd() + 1721119.5;
		int c2 = (int) floor((4*x2+3)/146097); 
		int x1 = (int) x2 - floor(146097*(c2/4));
		int c1 = (int) floor((100*x1 + 99)/36525);
		int	x0 = (int) x1 - floor(36525*c1/100);

		int y = 100*c2 + c1;
		int m = floor((5*x0 + 461)/153);
		int d = x0 - floor((153*m -457)/5) + 1;
	
		return ymd_t(y,m,d);
	}

	void Gregorian::set_mjd_from_ymd(const ymd_t &ymd) {
		int a = (14-ymd.m)/2;
		int y = ymd.y + 4800 - a;
		int m = ymd.m + 12*a - 3;

		_mod_julian_day = ymd.d + (153*m + 2)/5 + 365*y + (y/4) - (y/100) + (y/400) - 32045;

	}
};
