#ifndef LAB2_JULIAN_H
#define LAB2_JULIAN_H

#include "ansi_date.h"

namespace lab2 {
	class Julian : public AnsiDate {
		public:
			Julian();
			Julian(int year, int month, int day);
			Julian(const Date &date);
			Julian(Date * &date);

			virtual const int week_day() const;

		private:
			const float mjd_to_jd() const;
			virtual const bool is_leap_year(int year) const;
			ymd_t mjd_to_ymd() const;
			virtual void set_mjd_from_ymd(const ymd_t &ymd);
	};
}
#endif
