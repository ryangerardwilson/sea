CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -I../include -g  # ../include, not sea/include—get with the program
LIBS = -L../lib -lsea -lmicrohttpd -lsqlite3      # ../lib too, genius
all: sea-lib myapp
	rm -f *.o  # Nuke the .o files after building, because apparently you can't handle intermediates

sea-lib:
	cd .. && $(MAKE) lib  # cd .. to the actual sea dir, not some fantasy subdir

myapp: main.o sea-lib
	$(CC) main.o -o app $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:  # Full wipe: .o and app, for when you really fuck up
	rm -f *.o app
	cd .. && $(MAKE) clean  # Again, cd .., not cd sea—do I have to draw you a fucking map?

.PHONY: all sea-lib clean
