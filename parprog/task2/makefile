all: run

run: main
	OMP_NUM_THREADS=100 ./main 10000

main: main.cpp
	g++ -fopenmp main.cpp -o main

clean:
	rm -rf *.o main