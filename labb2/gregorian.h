#ifndef LAB2_GREGORIAN_H
#define LAB2_GREGORIAN_H

#include "ansi_date.h"
#include <string>

namespace lab2 {
	class Gregorian : public AnsiDate {
		public:
			Gregorian();
			Gregorian(int year, int month, int day);
			virtual ~Gregorian();

			virtual const int week_day() const;
			virtual const int days_this_month() const;

			virtual Gregorian &operator +=(const int n);
			virtual Gregorian &operator -=(const int n);

	//		Gregorian operator++(int);

			virtual void add_month(const int months);
			virtual void add_year(const int years);

		private:
			float mjd_to_jd() const;
			const bool is_leap_year(int y) const;
			ymd_t mjd_to_ymd() const;
			void set_mjd_from_ymd(const ymd_t &ymd);
	};	
};
#endif
