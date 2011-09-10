#include "kattistime.h"
#include "calendar.h"
#include "gregorian.h"
#include "julian.h"
#include <assert.h>

#include <iostream>
using namespace lab2;

int main() {

	//Init kattistime
   time_t mytime;
   time(&mytime);
   set_k_time(mytime);

	Calendar<Gregorian> cal;
	assert(cal.set_date(2000, 12, 2));
	assert(cal.add_event("Basketträning", 4, 12, 2000));
	assert(cal.add_event("Basketträning", 11, 12, 2000));
	assert(cal.add_event("Nyårsfrukost", 1, 1, 2001));
	assert(cal.add_event("Första advent", 1));
	// år = 2000, månad = 12
	assert(cal.add_event("Vårdagjämning", 20, 3));
	// år = 2000
	assert(cal.add_event("Julafton", 24, 12));
	assert(cal.add_event("Kalle Anka hälsar god jul", 24)); // också på julafto)n
	assert(cal.add_event("Julafton", 24)==false); // En likadan händelse samma datum sk)a
	// ignoreras och inte sättas in i kalendern
	assert(cal.add_event("Min första cykel", 20, 12, 2000));
	assert(cal.remove_event("Basketträning", 4));

	std::cout << "----------------------------------------" << std::endl;
	std::cout << cal; // OBS! Vårdagjämning och första advent är
	// före nuvarande datum och skrivs inte ut
	std::cout << "----------------------------------------" << std::endl;
	assert(cal.remove_event("Vårdagjämning", 20, 3, 2000));
	assert(cal.remove_event("Kalle Anka hälsar god jul", 24, 12, 2000));
	assert(cal.set_date(2000, 11, 2));
	assert(cal.remove_event("Julafton", 24) == false);
	std::cout << cal;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "This should have same events but with different dates--" << std::endl;
	Calendar<Julian> jcal;
	jcal = cal;
	std::cout << jcal <<std::endl;
	std::cout << "----------------------------------------" << std::endl;

}
