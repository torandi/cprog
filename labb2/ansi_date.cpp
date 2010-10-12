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

	AnsiDate::AnsiDate() : _year(0), _month(0), _day(0) { }
	AnsiDate::AnsiDate(int y,int m, int d) : _year(y), _month(m), _day(d) {}
	AnsiDate::~AnsiDate() {}

	int AnsiDate::days_this_month() const {
		return days_per_month[_month];
	}

	std::string AnsiDate::week_day_name() const {
		return weekdays[week_day()];
	}

	std::string AnsiDate::month_name() const {
		return months[_month];
	}

	Date& AnsiDate::operator=(const Date& date) {
		_day=date.day();
		_month=date.month();
		_year=date.year();
		return *this;
	}

	bool AnsiDate::operator==(const Date& date) const {
		return ( _year==date.year() &&
					_month==date.month() &&
					_day==date.day() );
	}
}
