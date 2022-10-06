all: run

run: main
	OMP_NUM_THREADS=2 ./main 1000

main: main.cpp
	g++ -fopenmp main.cpp -o main

clean:
	rm -rf *.o main