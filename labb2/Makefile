CFLAGS=-O2 -pg -g -Wall

all: ansi_date.o date.o gregorian.o julian.o datetest calendar_test cprog09lab22a cprog09lab22b cprog09lab23

#date: date.o
#	g++ date.o $(CFLAGS) -o test_template_vec

date.o: date.cpp date.h
	g++ $(CFLAGS) -c date.cpp -o date.o

ansi_date.o:  ansi_date.cpp ansi_date.h
	g++ $(CFLAGS) -c ansi_date.cpp -o ansi_date.o

gregorian.o: gregorian.cpp gregorian.h
	g++ $(CFLAGS) -c gregorian.cpp -o gregorian.o

julian.o: julian.cpp julian.h
	g++ $(CFLAGS) -c julian.cpp

cprog09lab22a.o: cprog09lab22a.cpp
	g++ $(CFLAGS) -c cprog09lab22a.cpp

cprog09lab22b.o: cprog09lab22b.cpp
	g++ $(CFLAGS) -c cprog09lab22b.cpp

cprog09lab23.o: cprog09lab23.cpp calendar.h
	g++ $(CFLAGS) -c cprog09lab23.cpp

datetest.o: datetest.cpp
	g++ $(CFLAGS) -c datetest.cpp

kattistime.o: kattistime.cpp kattistime.h
	g++ $(CFLAGS) -c kattistime.cpp

calendar_test.o: calendar_test.cpp calendar.h
	g++ $(CFLAGS) -c calendar_test.cpp

datetest: date.o ansi_date.o gregorian.o julian.o datetest.o kattistime.o
	g++ $(CFLAGS) date.o ansi_date.o gregorian.o julian.o datetest.o kattistime.o -o datetest

cprog09lab22a: date.o ansi_date.o gregorian.o julian.o cprog09lab22a.o kattistime.o
	g++ $(CFLAGS) date.o ansi_date.o gregorian.o julian.o cprog09lab22a.o kattistime.o -o cprog09lab22a

cprog09lab22b: date.o ansi_date.o gregorian.o julian.o cprog09lab22b.o kattistime.o
	g++ $(CFLAGS) date.o ansi_date.o gregorian.o julian.o cprog09lab22b.o kattistime.o -o cprog09lab22b

cprog09lab23: date.o ansi_date.o gregorian.o julian.o cprog09lab23.o kattistime.o 
	g++ $(CFLAGS) date.o ansi_date.o gregorian.o julian.o cprog09lab23.o kattistime.o -o cprog09lab23

calendar_test: date.o ansi_date.o gregorian.o julian.o kattistime.o calendar_test.o
	g++ $(CFLAGS) date.o ansi_date.o gregorian.o julian.o kattistime.o calendar_test.o -o calendar_test

clean:
	rm ansi_date.o date.o gregorian.o julian.o kattistime.o datetest.o datetest calendar_test.o calendar_test

submit_all: submit_22a submit_22b submit_23

submit_22a: cprog09lab22a.cpp date.cpp date.h gregorian.cpp gregorian.h julian.cpp julian.h kattistime.cpp kattistime.h ansi_date.cpp ansi_date.h 
	kattis.py -p oldkattis:cprog09lab22a cprog09lab22a.cpp date.cpp date.h gregorian.cpp gregorian.h julian.cpp julian.h kattistime.cpp kattistime.h ansi_date.cpp ansi_date.h

submit_22b: cprog09lab22b.cpp date.cpp date.h gregorian.cpp gregorian.h julian.cpp julian.h kattistime.cpp kattistime.h ansi_date.cpp ansi_date.h 
	kattis.py -p oldkattis:cprog09lab22b cprog09lab22b.cpp date.cpp date.h gregorian.cpp gregorian.h julian.cpp julian.h kattistime.cpp kattistime.h ansi_date.cpp ansi_date.h

submit_23: cprog09lab23.cpp date.cpp date.h gregorian.cpp gregorian.h julian.cpp julian.h kattistime.cpp kattistime.h ansi_date.cpp ansi_date.h 
	kattis.py -p oldkattis:cprog09lab23 cprog09lab23.cpp date.cpp date.h gregorian.cpp gregorian.h julian.cpp julian.h kattistime.cpp kattistime.h ansi_date.cpp ansi_date.h calendar.h
