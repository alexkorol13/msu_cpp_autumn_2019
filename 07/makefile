all: prog

prog: test.o Vector.h
	g++ -o prog test.o Vector.h

test.o: test.cpp Vector.h
	g++ -c -O2 -std=c++14 test.cpp

test:
	./prog

clean:
	rm -rf *.o prog