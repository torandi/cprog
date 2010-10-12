CFLAGS=-O2 -pg -g -Wall

all: ansi_date.o

#date: date.o
#	g++ date.o $(CFLAGS) -o test_template_vec

ansi_date.o: date.h ansi_date.cpp ansi_date.h
	g++ -c ansi_date.cpp date.h -o ansi_date.o

clean: 
	rm ansi_date.o
