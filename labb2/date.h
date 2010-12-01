#ifndef LAB2_DATE_H
#define LAB2_DATE_H

#include <string>
#include <iostream>

namespace lab2 {
	class Date {
		protected:
			int _mod_julian_day;

		public:
			virtual ~Date() {};

			/************************************************************
			 * Access functions.
			 ************************************************************/

			virtual const int mod_julian_day() const { return _mod_julian_day; };

			virtual const int day() const = 0;
			virtual const int month() const = 0;
			virtual const int year() const = 0;
			virtual const int week_day() const = 0;
			virtual const int days_per_week() const = 0;
			virtual const int days_this_month() const = 0;
			virtual const int months_per_year() const = 0;
			virtual const std::string week_day_name() const = 0;
			virtual const std::string month_name() const = 0;

			/************************************************************
			 * Operators
			 ************************************************************/


			virtual Date& operator=(const Date &date);
			virtual bool operator==(const Date &date) const;
			virtual bool operator!=(const Date &date) const;
			virtual bool operator<(const Date &date) const;
			virtual bool operator>(const Date &date) const;
			virtual bool operator<=(const Date &date) const;
			virtual bool operator>=(const Date &date) const;

			virtual Date &operator+=(const int n);
			virtual Date &operator-=(const int n);

			virtual Date &operator++();
			virtual Date &operator--();

			virtual int operator-(const Date &date) const;


			/************************************************************
			 * Modifiers
			 ************************************************************/
			virtual void add_year(int n = 1) = 0;
			virtual void add_month(int n = 1) = 0;

	};
}

std::ostream & operator<<(std::ostream & os, const lab2::Date &date);
#endif
