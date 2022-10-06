all: run

run: main
	OMP_NUM_THREADS=12 ./main 

main: main.cpp
	g++ -fopenmp main.cpp -o main

clean:
	rm -rf *.o main