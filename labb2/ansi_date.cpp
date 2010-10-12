#include "ansi_date.h"

#include <string>

namespace lab2 {
	static const std::string AnsiDate::weekdays[7]={
		"monday",
		"tuesday",
		"wednesday",
		"thursday",
		"friday",
		"saturday",
		"sunday"
	};

	static const std::string AnsiDate::months[12]={
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
	static const int AnsiDate::days_per_month[12]={31,28,31,30,31,30,31,31,30,31,30,31};

	AnsiDate::AnsiDate() : year(0), month(0), day(0) { }
	AnsiDate::AnsiDate(int y,int m, int d) : year(y), month(m), day(d) {}
	AnsiDate::~AnsiDate() {}

	int AnsiDate::days_this_month() const {
		return days_per_month[month];
	}

	std::string AnsiDate::week_day_name() const {
		return weekdays[week_day()];
	}

	std::string AnsiDate::month_name() const {
		return months[month];
	}

	Date& AnsiDate::operator=(const Date &date) {
		day=date.day();
		month=date.month();
		year=date.year();
		return *this;
	}

	bool AnsiDate::operator==(const Date &date) {
		return ( year==date.year() &&
					month==date.month() &&
					day==date.day() );
	}
}
