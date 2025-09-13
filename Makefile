CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -Isea/include -g
LIBS = -Lsea/lib -lsea -lmicrohttpd -lsqlite3

all: sea-lib myapp

sea-lib:
	cd sea && $(MAKE) lib

myapp: main.o sea-lib
	$(CC) main.o -o myapp $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	cd sea && $(MAKE) clean

.PHONY: all sea-lib clean
