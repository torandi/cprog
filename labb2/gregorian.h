#ifndef LAB2_GREGORIAN_H
#define LAB2_GREGORIAN_H

#include "ansi_date.h"

namespace lab2 {
	class Gregorian : public AnsiDate {
		public:
			Gregorian();
			Gregorian(const Date &date);
			Gregorian(int year, int month, int day);

	//		Gregorian operator++(int);


		private:
			virtual const bool is_leap_year(int y) const;
			virtual ymd_t mjd_to_ymd() const;
			virtual void set_mjd_from_ymd(const ymd_t &ymd);
			const float mjd_to_jd() const;
	};	
};
#endif
