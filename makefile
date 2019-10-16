all: calc

run: calc
	./calc

test: calc
	python3 ./test_calc.py

calc: main.cpp
	g++ -o calc main.cpp

clean:
	rm -rf *.o calc