all: run

run: main
	OMP_NUM_THREADS=6 ./main 

main: main.cpp
	g++ -Wall -fopenmp main.cpp -o main

clean:
	rm -rf *.o main