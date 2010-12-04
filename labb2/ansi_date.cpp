#include "ansi_date.h"
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <algorithm>
 
namespace lab2 {
	const std::string AnsiDate::weekdays[7]={
		"monday",
		"tuesday",
		"wednesday",
		"thursday",
		"friday",
		"saturday",
		"sunday"
	};

	const std::string AnsiDate::months[12]={
		"january",
		"february",
		"march",
		"april",
		"may",
		"june",
		"july",
		"august",
		"september",
		"october",
		"november",
		"december"
	};
	const int AnsiDate::days_per_month[12]={31,28,31,30,31,30,31,31,30,31,30,31};

	AnsiDate::AnsiDate() { }
	AnsiDate::~AnsiDate() {}

	const std::string AnsiDate::week_day_name() const {
		return weekdays[week_day()-1];
	}

	const std::string AnsiDate::month_name() const {
		return months[month()-1];
	}

	void AnsiDate::add_month(const int months) {
		if(months>0) {
			for(int i=0;i<months;++i) {
				ymd_t ymd=mjd_to_ymd();
				++ymd.m;
				if(ymd.m>12) { 
					ymd.m=1;
					++ymd.y;
				}
				if(ymd.d > days_this_month(ymd.y,ymd.m)) {
					//Just add 30 days
					_mod_julian_day+=30;
				} else {
					set_mjd_from_ymd(ymd);
				}
			}
		} else {
			//Bloody negative months...
			for(int i=0;i>months;--i) {
				ymd_t ymd=mjd_to_ymd();
				--ymd.m;
				if(ymd.m<1) { 
					ymd.m=12;
					--ymd.y;
				}
				if(ymd.d > days_this_month(ymd.y,ymd.m)) {
					_mod_julian_day-=30;
				} else {
					set_mjd_from_ymd(ymd);
				}
			}
		}
	}

	void AnsiDate::add_year(const int years) {
		ymd_t ymd=mjd_to_ymd();
		ymd.y+=years;
		if(ymd.m==2 && ymd.d==29 && !is_leap_year(ymd.y))
			ymd.d=28;
		set_mjd_from_ymd(ymd);
	}

	const int AnsiDate::day() const {
		return mjd_to_ymd().d;
	}

	const int AnsiDate::month() const {
		return mjd_to_ymd().m;
	}

	const int AnsiDate::year() const {
		return mjd_to_ymd().y;
	}

	bool AnsiDate::self_test() {
		ymd_t ymd=mjd_to_ymd();
		set_mjd_from_ymd(ymd);
		ymd_t ymd2=mjd_to_ymd();
		return (ymd.y==ymd2.y && ymd.m==ymd2.m && ymd.d == ymd2.d);
	}

	const int AnsiDate::days_this_month() const {
		ymd_t ymd = mjd_to_ymd();
		return days_this_month(ymd.y,ymd.m);
	}
		

	const int AnsiDate::days_this_month(int year,int month) const {
		if((month == 2) && is_leap_year(year)) {
			return 29;
		} else {
			return days_per_month[month-1];
		}
	}

	
	void AnsiDate::check_valid_date(const ymd_t ymd) const {
		//Check month:
		if(ymd.m<1 || ymd.m > 12) {
			throw std::out_of_range("Month must be a number between 1 and 12");
		}
		if(ymd.d<1 || ymd.d > days_this_month(ymd.y,ymd.m) ) {
			throw std::out_of_range("Invalid number of days for this month");
		}
	}

	const int AnsiDate::week_day() const {
		if (_mod_julian_day < 0) {
			return 7 - ((abs(_mod_julian_day+3))%7);
		} else {
			return ((_mod_julian_day+2)%7)+1;
		}
	}
}
