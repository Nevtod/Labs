all: run

run: main
	OMP_NUM_THREADS=1 ./main 1000000

main: main.cpp
	g++ -Wall -fopenmp main.cpp -o main

clean:
	rm -rf *.o main