CC = gcc
CFLAGS = -Wall -Werror -O2

all: main.o
	$(CC) -o host main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c


.PHONY: clean

clean:
	-rm -f *.o lz77
