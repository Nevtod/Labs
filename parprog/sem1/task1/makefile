all: run

run: main
	mpirun -np 6 ./main

main: main
	mpicc -std=c99 main.c -o main
	chmod u+x main


clean:
	rm -rf *.o main