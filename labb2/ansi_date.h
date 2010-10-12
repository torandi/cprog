#ifndef LAB2_ANSI_DATE_H
#define LAB2_ANSI_DATE_H
#include <string>
#include "date.h"

namespace lab2 {
	class AnsiDate : public Date {
		static const std::string weekdays[7];
		static const std::string months[12];
		static const int days_per_month[12];
		protected:
			int _year;
			int _day;
			int _month;

			AnsiDate();
			AnsiDate(int y,int m, int d);
			virtual ~AnsiDate();


		public:
			virtual int day() const { return _day; };
			virtual int month() const { return _month; };
			virtual int year() const { return _year; };
			virtual int days_per_week() const { return 7; };
			virtual int months_per_year() const { return 12; };
			virtual int days_this_month() const;
			virtual std::string week_day_name() const;
			virtual std::string month_name() const;

			virtual Date& operator=(const Date &date);
			virtual bool operator==(const Date &date) const;
	};
};
#endif
