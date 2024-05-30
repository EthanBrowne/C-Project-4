CC = gcc
CFLAGS = -Wall -std=c99 -g
vinyl: vinyl.o inventory.o input.o

vinyl.o: vinyl.c inventory.h input.h
inventory.o: inventory.c inventory.h input.h
input.o: input.c input.h

vinyl.c: inventory.h input.h
inventory.c: inventory.h input.h
input.c: input.h

clean:
	rm -f vinyl vinyl.o inventory.o input.o