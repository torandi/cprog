CFLAGS=-O2 -pg -g -Wall

all: test_template_vec test

test_template_vec: test_template_vec.cpp Vector.h
	g++ test_template_vec.cpp Vector.h $(CFLAGS) -o test_template_vec

test: test.cpp Vector.h

clean: 
	rm Vector.o test.o vector_test test_vec.o
