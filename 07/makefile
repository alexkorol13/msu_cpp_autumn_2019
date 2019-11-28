all: prog

prog: main.o
	g++ -o prog

main.o: main.cpp
	g++ main.cpp -c --std=c++14 -O2

test: test.o
	g++ -o prog

test.o:
	g++ test.cpp -c --std=c++14 -O2

clean:
	rm -rf *.o prog