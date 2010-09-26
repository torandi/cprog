CFLAGS=-O2 -pg -g -Wall

all: test_template_vec test cprog09lab15

test_template_vec: test_template_vec.cpp vector.h
	g++ test_template_vec.cpp $(CFLAGS) -o test_template_vec

test: test.cpp vector.h
	g++ test.cpp $(CFLAGS) -o test

cprog09lab15: cprog09lab15.cpp vector.h
	g++ cprog09lab15.cpp $(CFLAGS) 

clean: 
	rm test.o test test_template_vec cprog09lab15
