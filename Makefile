CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -Isea/include -g
LIBS = -Lsea/lib -lsea -lmicrohttpd -lsqlite3
myapp: main.o
	$(CC) main.o -o myapp $(LIBS)
main.o: main.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o myapp
.PHONY: clean
