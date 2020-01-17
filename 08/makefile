all: prog

prog: main.o ThreadPool.h
	g++ -pthread -o prog main.o ThreadPool.h

main.o: main.cpp ThreadPool.h
	g++ -pthread -c -O2 --std=c++17 main.cpp

test:
	./prog

clean:
	rm -rf *.o prog