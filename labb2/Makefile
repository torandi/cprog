CFLAGS=-O2 -pg -g -Wall

all: ansi_date.o date.o gregorian.o julian.o datetest

#date: date.o
#	g++ date.o $(CFLAGS) -o test_template_vec

date.o: date.cpp date.h
	g++ -c date.cpp -o date.o

ansi_date.o:  ansi_date.cpp ansi_date.h
	g++ -c ansi_date.cpp -o ansi_date.o

gregorian.o: gregorian.cpp gregorian.h
	g++ -c gregorian.cpp -o gregorian.o

julian.o: julian.cpp julian.h
	g++ -c julian.cpp

cprog09lab22a.o: cprog09lab22a.cpp
	g++ -c cprog09lab22a.cpp

datetest.o: datetest.cpp
	g++ -c datetest.cpp

kattistime.o: kattistime.cpp kattistime.h
	g++ -c kattistime.cpp

datetest: date.o ansi_date.o gregorian.o julian.o datetest.o kattistime.o
	g++ date.o ansi_date.o gregorian.o julian.o datetest.o kattistime.o -o datetest

cprog09lab22a: date.o ansi_date.o gregorian.o julian.o cprog09lab22a.o kattistime.o
	g++ date.o ansi_date.o gregorian.o julian.o cprog09lab22a.o kattistime.o -o cprog09lab22a

clean: 
	rm ansi_date.o date.o gregorian.o julian.o
