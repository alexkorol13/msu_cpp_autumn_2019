all: prog

prog: main.o
	g++ -o prog main.o

main.o: main.cpp
	g++ -c -O2 -std=c++14 main.cpp

test: prog
	./prog

clean:
	rm -rf *.o prog