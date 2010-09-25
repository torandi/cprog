CFLAGS=-O2 -pg -g -Wall

all: vector_test

Vector.o: Vector.cpp Vector.h
	g++ -c Vector.cpp $(CFLAGS) -o Vector.o

test.o: test.cpp
	g++ -c test.cpp $(CFLAGS) -o test.o

test_vec.o: test_template_vec.cpp
	g++ -c test_template_vec.cpp $(CFLAGS) -o test_vec.o

vector_test: Vector.o test_vec.o
	g++ Vector.o test_vec.o $(CFLAGS) -o vector_test

clean: 
	rm Vector.o test.o vector_test test_vec.o
