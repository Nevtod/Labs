all: run

run: build
	OMP_NUM_THREADS=6 ./main 

build: main.cpp
	g++ -Wall -fopenmp main.cpp -o main

clean:
	rm -rf *.o main