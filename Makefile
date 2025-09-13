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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

LIBS = -lmicrohttpd -lsqlite3  # Link these in your apps too

clean:
	rm -f $(CORE_OBJS) lib/libsea.a 

