all: run

run: main
	mpirun -np 500 ./main

main: main.c
	mpicc -std=c99 main.c -o main
	chmod u+x main


clean:
	rm -rf *.o main