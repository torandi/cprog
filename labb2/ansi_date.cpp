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

}
