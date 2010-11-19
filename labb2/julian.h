#ifndef LAB2_JULIAN_H
#define LAB2_JULIAN_H

#include "ansi_date.h"

namespace lab2 {
	class Julian : public AnsiDate {
		public:
			Julian();
			Julian(int year, int month, int day);

			virtual const int week_day() const;
			virtual const int days_this_month() const;

			virtual Julian &operator +=(const int n);
			virtual Julian &operator -=(const int n);

		private:
			const float mjd_to_jd() const;
			ymd_t mjd_to_ymd() const;
			virtual void set_mjd_from_ymd(const ymd_t &ymd);
	};
}
#endif
