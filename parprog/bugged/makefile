name = bugged6.c

all: run
run: main
	OMP_NUM_THREADS=6 ./main 

main: $(name)
	gcc -Wall -fopenmp $(name) -o main

clean:
	rm -rf *.o main