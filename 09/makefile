all: prog

prog: main.o sort.o
	g++ -pthread -o prog main.o sort.o

main.o: main.cpp sort.h
	g++ -pthread -c -O2 --std=c++17 main.cpp

sort.o: sort.cpp
	g++ -pthread -c -O2 --std=c++17 sort.cpp

test:
	./prog

clean:
	rm -rf *.o prog
