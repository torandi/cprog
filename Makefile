CFLAGS=-O2 -pg -g -Wall

all: vector_test

Vector.o: Vector.cpp Vector.h
	g++ -c Vector.cpp $(CFLAGS) -o Vector.o

test.o: test.cpp
	g++ -c test.cpp $(CFLAGS) -o test.o

vector_test: Vector.o test.o
	g++ Vector.o test.o $(CFLAGS) -o vector_test

clean: 
	rm Vector.o test.o vector_test
