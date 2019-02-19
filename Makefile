all: sem.c sem.h
	gcc sem.c -pthread -lrt -o sem
	
run:
	./sem

clean:
	rm sem

