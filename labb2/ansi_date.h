#ifndef LAB2_ANSI_DATE_H
#define LAB2_ANSI_DATE_H
#include <string>
#include "date.h"

#define DAYS_BETWEEN_UNIX_AND_MOD_JULIAN 40587

namespace lab2 {
	class AnsiDate : public Date {
		static const std::string weekdays[7];
		static const std::string months[12];
		static const int days_per_month[12];
		protected:
			AnsiDate();
			virtual ~AnsiDate();


		public:
			virtual int days_per_week() const { return 7; };
			virtual int months_per_year() const { return 12; };
			virtual int days_this_month() const;
			virtual std::string week_day_name() const;
			virtual std::string month_name() const;
	};
};
#endif
