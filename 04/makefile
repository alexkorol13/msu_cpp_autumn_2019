all: prog

prog: main.o BigInt.o
	g++ -o prog $^

main.o: main.cpp BigInt.h
	g++ main.cpp -c --std=c++14 -O2

BigInt.o: BigInt.cpp
	g++ BigInt.cpp -c --std=c++14 -O2

test: prog
	./prog

clean:
	rm -rf *.o *.dSYM prog