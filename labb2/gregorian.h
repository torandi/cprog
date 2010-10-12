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

			virtual week_day() const;
			virtual days_this_month() const;

			virtual Gregorian &operator +=(const int n);
			virtual Gregorian &operator -=(const int n);
	};	
};
#endif
