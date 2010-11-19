#include "ansi_date.h"
#include <string>
 
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
		"mars",
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
		return weekdays[week_day()];
	}

	const std::string AnsiDate::month_name() const {
		return months[month()];
	}

	Date& AnsiDate::operator=(const Date &date) {
		_mod_julian_day=date.mod_julian_day();
		return *this;
	
	}

	bool AnsiDate::operator==(const Date &date) const {
		return date.mod_julian_day()==mod_julian_day();
	}	

	bool AnsiDate::operator!=(const Date &date) const {
		return date.mod_julian_day()!=mod_julian_day();
	}

	void AnsiDate::add_month(const int months) {
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

}
