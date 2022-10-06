all: run

run: main
	OMP_NUM_THREADS=20 ./main 15

main: main.cpp
	g++ -fopenmp main.cpp -o main

clean:
	rm -rf *.o main