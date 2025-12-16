CC = c:\msys64\opt\bin\x86_64-w64-mingw32-gcc.exe
CFLAGS = -Wall -Werror -O2
TARGET = $(notdir $(CURDIR))

all: main.o
	$(CC) -o $(TARGET) main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

.PHONY: clean

clean:
	-rm -f *.o *.exe
