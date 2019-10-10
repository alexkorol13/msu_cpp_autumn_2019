CC = g++ -c -std=c++14 -O2

all: result

result: main.o Timer.o
	g++ -o result main.o Timer.o

Timer.o: Timer.cpp Timer.h
	$(CC) Timer.cpp

main.o: main.cpp Timer.cpp
	$(CC) main.cpp

clean:
	rm -rf *.o result

run: 
	./result
