ARGS ?= default_args

all: calc

calc: main.o
	g++ -o calc main.o

main.o: main.cpp
	g++ -c -O2 -std=c++14 main.cpp

run: calc
	./calc $(ARGS)

test:
	python3 ./test_calc.py

clean:
	rm -rf *.o calc