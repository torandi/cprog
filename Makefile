CFLAGS=-O2 -pg -g -Wall

all: vector_simple

vector_simple : test_template_vec.cpp Vector.h
	g++ test_template_vec.cpp Vector.h $(CFLAGS) -o vector_simple

clean: 
	rm Vector.o test.o vector_test test_vec.o
