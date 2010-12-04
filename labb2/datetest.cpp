// projektlokala headerfiler
#include "kattistime.h"     
#include "julian.h"         
#include "gregorian.h"      

// STL headerfiler
#include <iostream>
#include <assert.h>             // assert(b) ger felmeddelande om b falsk
#include <ctime>


// Obs att testerna frutstter att dina klasser returnerar
// mnader och dagar indexerade frn ETT. Testerna r inte
// fullstndiga


int main()
{
    using namespace lab2;        // Exponera funktionalitet i namnrymden lab2

    ////////////////////////////////////////////////////////////
    // Stt tiden. OBS skicka inte nedanstende kod till kattis
    time_t mytime;
    time(&mytime);
    set_k_time(mytime);
    ////////////////////////////////////////////////////////////

    
    Julian tj;                  // ok: defaultkonstruktor ger dagens datum
    Gregorian gtoday;           // ok fr gregorian ocks
    std::cout << "Idag är det den " << gtoday << std::endl;
	 std::cout << "Running test 1...";
    assert(tj - gtoday == 0);
	 std::cout << "OK" << std::endl;
    Gregorian tg(2006, 10, 31); // ok: stt datum explicit
                                // fljande fungerar ocks:
                                // Gregorian(2000, Gregorian::October, 31)
    Date &j = tj;               // tkomst av funktioner genom Dates interface
    Date &g = tg;       
    Date &today = gtoday;

    {
      Julian j1;
      Date & d1 = j1;
      Julian j2(d1);
      Date * dp = &j2;
      Julian j3(dp);
    }
     
    time_t tp;
    time(&tp);
    struct tm *t = gmtime(&tp);
    int year  = t->tm_year + 1900;
    int month = t->tm_mon + 1;      // mnaderna och dagarna
    int day   = t->tm_mday;         // indexerade frn ETT

	 
	 /*
	 Julian jl;
	 while(jl>Julian(1800,1,1)) {
		--jl;
		std::cout<<jl<<" ("<<jl.mod_julian_day()<<")"<<"...";
		if(!jl.self_test()) {
			std::cout<<"FAIL! (is now: "<<jl<<")"<<std::endl;
			return 1;
		} else {
			std::cout<<"OK"<<std::endl;
		}
	 }
	*/
    std::cout << "Testing constructors..." << std::endl;
	 Gregorian gt(2010,12,1);
	 Julian jt(2010,12,1);
	 std::cout << "Gregorian (2010-12-01): "<<gt<<std::endl;
	 std::cout << "Julian (2010-12-01): "<<jt<<std::endl;

	 std::cout << "Gregorian today: "<<Gregorian()<<std::endl;
	 std::cout << "Julian today: "<<Julian()<<std::endl;

	 std::cout << "Running gregorian selftest...";
	 assert(gt.self_test());
	 std::cout << "OK" << std::endl;

	 std::cout << "Running julian selftest...";
	 assert(jt.self_test());
	 std::cout << "OK" << std::endl;


	 std::cout << "Running pre-test 1...";
    assert(gt.year() == 2010 &&              
           gt.month() == 12 &&
           gt.day() == 1);                  
	 std::cout << "OK" << std::endl;
	 gt=Gregorian(1858,1,1);
	 std::cout << "Gregorian (1858-01-01): "<<gt<<std::endl;
	 assert(gt.self_test());
	 std::cout << "Running pre-test 2...";
    assert(gt.year() == 1858 &&              
           gt.month() == 1 &&
           gt.day() == 1);                  
	 std::cout << "OK" << std::endl;

	 std::cout << "Running pre-test 3...";
    assert(jt.year() == 2010 &&              
           jt.month() == 12 &&
           jt.day() == 1);                  
	 std::cout << "OK" << std::endl;
	 jt=Julian(1858,1,1);
	 std::cout << "Julian (1858-01-01): "<<jt<<std::endl;
	 assert(jt.self_test());
	 std::cout << "Running pre-test 4...";
    assert(gt.year() == 1858 &&              
           gt.month() == 1 &&
           gt.day() == 1);                  
	 std::cout << "OK" << std::endl;

	 std::cout << "Running test 2...";
    assert(today.year() == year &&          // rtt initierad
           today.month() == month &&
           today.day() == day);
	 std::cout << "OK" << std::endl;
	 std::cout << "Running test 3...";
    assert(g.year() == 2006 &&              // rtt initierad
           g.month() == 10 &&
           g.day() == 31);                  // obs! ettindexerade
	 std::cout << "OK" << std::endl;
    
    std::cout << "Testing access..." << std::endl;
	 std::cout << "Running test 4...";
    assert(g.days_per_week() == 7);         // rtt antal dagar per vecka
	 std::cout << "OK" << std::endl;
	 std::cout << "Running test 5...";
    assert(j.days_per_week() == 7);         // rtt antal dagar per vecka
	 std::cout << "OK" << std::endl;
	 std::cout << "Running test 6...";
    assert(g.days_this_month() == 31);      // rtt antal dagar denna mnad
	 std::cout << "OK" << std::endl;
	 std::cout << "Running test 7...";
    assert(g.months_per_year() == 12);      // rtt antal mnader per r
	 std::cout << "OK" << std::endl;
	 std::cout << "Running test 8...";
    assert(j.months_per_year() == 12);      // rtt antal mnader per r
	 std::cout << "OK" << std::endl;
	 std::cout << "Running test 9...";
    assert(g.week_day() == 2); // rtt veckodag
	 std::cout << "OK" << std::endl;

    std::cout << "Testing manipulation..." << std::endl;
    ++g;                                    // prefix kning
	 std::cout << "Running test 10...";
    assert(g.week_day() == 3); // rtt veckodag
	 std::cout << "OK" << std::endl;
    --g;                                    // prefix minskning
	 std::cout << "Running test 11...";
    assert(g.week_day() == 2); // rtt veckodag
	 std::cout << "OK" << std::endl;
    g += 2;                                 // lgg till tv dagar
	 std::cout << "Running test 12...";
    assert(g.week_day() == 4); // rtt veckodag
	 std::cout << "OK" << std::endl;
	 Gregorian g2 = g;
	 std::cout<<g<<std::endl;
    g -= 3;                     // dra bort tre dagar
    g.add_month();              // lgg till en mnad
    g.add_month(-1);            // dra bort en mnad    
    g.add_year(10);             // lgg till tio r.
    g += 3;                     // lägg till tre dagar

	 std::cout << "Running test 13...";
	 assert(g.month()==g2.month());
	 std::cout << "OK" << std::endl;
	 std::cout << "Running test 14...";
	 assert(g.year()==g2.year()+10);
	 std::cout << "OK" << std::endl;
	 
    
    std::cout << "Testing miscellaneous functions..." << std::endl;
    Julian jj(tj);              // kopieringskonstruktor
    const Gregorian gg;
    gg.year();                  // gg konstant, lsa gr bra
    g = gg;                     // tilldelning
    if(g == gg ||               // jmfrelse
       g != gg ||               // jmfrelse 
       g < gg ||                // jmfrelse 
       g >= gg)                 // jmfrelse 
        {}
    
    std::cout << "Testing boundary violations";
    Gregorian temp(1700, 1, 1);
    Date &d = temp;


    // loopa ver dagar och kolla att inga grnser ver/underskrids
    for(int i = 0; i < 100000; ++i, ++d)
        {
		  		
            if(!(i % 5000))        // utskrift p framsteg
                {
                    std::cout << ".";
                    flush(std::cout);
                }
            int m[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if(d.year() >= 1700 && 
               d.month() >= 1 && d.month() <= 12 &&
               d.day() >= 1 && d.day() <= m[d.month() - 1]
					&& d.week_day() >0 && d.week_day() <=7)
                {
					 	d.week_day_name();
						d.month_name();
					 }
            else
                {
                    std::cout << std::endl;
                    std::cout << "boundary checks failed: " << std::endl;
                    std::cout << d.year() << ", "
                              << d.month() << ", "
                              << d.day() << std::endl;
						  std::cout << "wd: " << d.week_day() <<std::endl;
                    return 1;
                }
        }
    std::cout << std::endl;

    std::cout << "Testing leap years..." << std::endl;
    // testa om skottren fr sekelskiften fungerar som de ska
    for(int y = 1958; y < 2500; y += 100)
        {
            Gregorian temp(y, 2, 28);
            Date &e = temp;
            ++e;                                   // lgg till en dag
            if((y % 400 == 0 && e.day() == 29) ||  // skottdag (obs! ETTindexerad)
               (y % 400 != 0 && e.day() == 1))     // ej skottdag
                {}
            else
                {
                    std::cout << std::endl << "leap day test failed: " << std::endl;
                    std::cout << e.year() << ", "
                              << e.month() << ", "
                              << e.day() << std::endl;
                    return 1;
                }
        }

    
	 g=Gregorian(1992,2,29);
	 g.add_month(29);
	 g.add_month(41);
	 std::cout<<"pre "<<g<<std::endl;
	 g.add_month(-19);
	 std::cout << "Running test 15...";
    if(g.year() == 1996 &&
           g.month() == 5 &&
           g.day() == 28) {}
	 else {
	 	std::cout<<"FAIL!: "<<g<<std::endl;
		return 1;
	 }
	 std::cout << "OK" << std::endl;

	 std::cout << "Running test 16...";
	 g=Gregorian(1858,1,1);
	 assert(g.week_day()==5);
	 ++g;
	 assert(g.week_day()==6);
	 ++g;
	 assert(g.week_day()==7);
	 ++g;
	 assert(g.week_day()==1);
	 ++g;
	 assert(g.week_day()==2);
	 ++g;
	 assert(g.week_day()==3);
	 ++g;
	 assert(g.week_day()==4);
	 ++g;
	 assert(g.week_day()==5);
	 std::cout << "OK" << std::endl;

    std::cout << std::endl << "All tests were successful." << std::endl;

    // fljande ska inte g att kompilera
#if 0
    gg.leap_year();             // fel: kan inte komma t protected/private medlem
    gg = g;                     // fel: kan inte tilldela konstant
    ++gg;                       // fel: kan inte ndra p en konstant
#endif

    return 0;
}

