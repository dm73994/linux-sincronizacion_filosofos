all: prodcons philosophers

prodcons: prodcons.o
	gcc -o prodcons prodcons.o -pthread

prodcons.o: prodcons.c
	gcc -c -o prodcons.o prodcons.c

philosophers: philosophers.o
	gcc -o philosophers philosophers.o -pthread

philosophers.o: philosophers.c
	gcc -c -o philosophers.o philosophers.c

clean:
	rm -f *.o prodcons philosophers
