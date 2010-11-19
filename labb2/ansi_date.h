#ifndef LAB2_ANSI_DATE_H
#define LAB2_ANSI_DATE_H
#include <string>
#include "date.h"

#define DAYS_BETWEEN_UNIX_AND_MOD_JULIAN 40587

namespace lab2 {
	struct ymd_t {
		int y;
		int m;
		int d;
		ymd_t() {};
		ymd_t(int _y,int _m,int _d) : y(_y), m(_m), d(_d) { };
	};

	class AnsiDate : public Date {
		private:
			static const std::string weekdays[7];
			static const std::string months[12];
			virtual ymd_t mjd_to_ymd() const = 0;
			virtual void set_mjd_from_ymd(const ymd_t &ymd) = 0;
			virtual const bool is_leap_year(int y) const = 0;
		protected:
			AnsiDate();
			virtual ~AnsiDate();
			static const int days_per_month[12];

		public:
			virtual const int days_per_week() const { return 7; };
			virtual const int months_per_year() const { return 12; };
			virtual const std::string week_day_name() const;
			virtual const std::string month_name() const;

			virtual const int day() const;
			virtual const int month() const;
			virtual const int year() const;

			virtual Date& operator=(const Date &date);
			virtual bool operator==(const Date &date) const;
			bool operator!=(const Date &date) const;

			virtual Date &operator+=(const int n);
			virtual Date &operator-=(const int n);

			virtual void add_month(const int months=1);
			virtual void add_year(const int years=1);
	};

};
#endif
