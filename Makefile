CFLAGS=-O2 -pg -g -Wall

all: test_template_vec test cprog09lab15

test_template_vec: test_template_vec.cpp vector.h
	g++ test_template_vec.cpp $(CFLAGS) -o test_template_vec

must_follow_a.o: must_follow_a.cpp must_follow_a.h
	g++ -c must_follow_a.cpp

test: must_follow_a.o
	/afs/nada.kth.se/info/cprog09/cxxtest/cxxtestgen.py --error-printer -o 1.2b.cpp test_must_follow_a.cpp
	g++ -o test_1.2b -I /afs/nada.kth.se/info/cprog09/cxxtest/ 1.2b.cpp must_follow_a.o
	./test_1.2b


cprog09lab15: cprog09lab15.cpp vector.h
	g++ cprog09lab15.cpp $(CFLAGS) -o  cprog09lab15

clean: 
	rm test.o test test_template_vec cprog09lab15
