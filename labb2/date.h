#ifndef LAB2_DATE_H
#define LAB2_DATE_H

#include <string>

namespace lab2 {
	class Date {
		public:
			virtual ~Date() {};

			/************************************************************
			 * Access functions.
			 ************************************************************/

			long mod_julian_day() const = 0;

			virtual int day() const = 0;
			virtual int month() const = 0;
			virtual int year() const = 0;
			virtual int week_day() const = 0;
			virtual int days_per_week() const = 0;
			virtual int days_this_month() const = 0;
			virtual int months_per_year() const = 0;
			virtual std::string week_day_name() const = 0;
			virtual std::string month_name() const = 0;

			/************************************************************
			 * Operators
			 ************************************************************/


			virtual int operator=(const Date &date) = 0;
			virtual int operator==(const Date &date) const = 0;
			int operator!=(const Date &date) const { return !(*this==date); };

			virtual Date &operator+=(const int n) = 0;
			virtual Date &operator-=(const int n) = 0;

			virtual Date &operator++() { return *this+=1; };
			virtual Date &operator--() { return *this-=1; };


			/************************************************************
			 * Modifiers
			 ************************************************************/

	};
};
#endif
