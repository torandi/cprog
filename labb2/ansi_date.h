#ifndef LAB2_ANSI_DATE_H
#define LAB2_ANSI_DATE_H
#include <string>
#include "date.h"

namespace lab2 {
	class AnsiDate : public Date {
		static const std::string weekdays[7];
		static const std::string months[12];

		protected:
			AnsiDate();
			AnsiDate(int year,int month, int day);
			virtual ~AnsiDate();

		public:
			virtual int day() const;
			virtual int month() const;
			virtual int year() const;
			virtual int days_per_week() const { return 7; };
			virtual int months_per_year() const { return 12; };
			virtual std::string week_day_name() const;
			virtual std::string month_name() const;

			virtual int operator=(const Date &date);
			virtual int operator==(const Date &date) const;
	}
}

#endif
