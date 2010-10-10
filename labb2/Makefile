CFLAGS=-O2 -pg -g -Wall

all: date.o

date: date.o
	g++ date.o $(CFLAGS) -o test_template_vec

date.o: date.h date.cpp
	g++ -c date.cpp

clean: 
	rm date.o date
