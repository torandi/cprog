CFLAGS=-O2 -pg -g

Vector.o: Vector.cpp Vector.h
	g++ -c Vector.cpp $(CFLAGS) -o Vector.o

clean: 
	rm Vector.o
