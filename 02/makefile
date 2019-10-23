all: prog

run: prog
	./prog

prog: main.o LinearAllocator.o
	g++ -o prog main.o LinearAllocator.o

test: test.o LinearAllocator.o
	g++ -o test test.o LinearAllocator.o
	./test

test.o: test.cpp LinearAllocator.h
	g++ -c -std=c++14 test.cpp
	
main.o: main.cpp LinearAllocator.h
	g++ -c -O2 -std=c++14 main.cpp

LinearAllocator.o: LinearAllocator.cpp
	g++ -c -O2 -std=c++14 LinearAllocator.cpp

clean:
	rm -rf *.o prog	