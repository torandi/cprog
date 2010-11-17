#ifndef LAB2_GREGORIAN_H
#define LAB2_GREGORIAN_H

#include "ansi_date.h"

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

		private:
			float mjd_to_jd() const;
			const bool is_leap_year(int y) const;
			ymd_t mjd_to_ymd() const;
	};	
};
#endif
