all: run

run: main
	./main 5

main: main.c
	gcc main.c -o main -lpthread -lrt
	
clean:
	rm -rf *.o main