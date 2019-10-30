CC = g++ -c -std=c++14 -O2

all: prog

run: prog
	./prog

prog: main.o Matrix.o
	g++ -o prog main.o Matrix.o

main.o: main.cpp matrix.h
	$(CC) main.cpp

Matrix.o: Matrix.cpp
	$(CC) Matrix.cpp

clean:
	rm -rf *.o prog