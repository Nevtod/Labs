all: build

run: main
	mpirun -np 10 ./main 100

build: main.c
	mpicc  main.c -o main -lm
	chmod u+x main

clean:
	rm -rf *.o main