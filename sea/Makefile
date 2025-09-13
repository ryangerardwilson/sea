CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -Iinclude -g
AR = ar
ARFLAGS = rcs

CORE_SRCS = src/http.c src/router.c src/model.c src/view.c src/sea.c
CORE_OBJS = $(CORE_SRCS:.c=.o)

# Build static lib
lib: lib/libsea.a

lib/libsea.a: $(CORE_OBJS)
	mkdir -p lib
	$(AR) $(ARFLAGS) $@ $(CORE_OBJS)
	@echo "Sea framework built: libsea.a—now link this shit in your apps."

# Demo binary (old standalone, for testing)
demo: demo/main.o lib/libsea.a
	$(CC) demo/main.o -o demo/sea -Llib -lsea $(LIBS)

demo/main.o: demo/main.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

LIBS = -lmicrohttpd -lsqlite3  # Link these in your apps too

clean:
	rm -f $(CORE_OBJS) demo/*.o lib/libsea.a demo/sea

.PHONY: lib demo clean
