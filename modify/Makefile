CFLAGS = -Wall -ansi -pedantic -O2 -ffast-math -funroll-loops \
	-Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DNDEBUG \
	-I. -I.. -DUSE_NET -D_BSD_SOURCE
LIBS = -lm
SRCS = gobpack.c jnbpack.c jnbunpack.c
OBJS = gobpack.o jnbpack.o jnbunpack.o
TARGETS = ../gobpack ../jnbpack ../jnbunpack

all: $(TARGETS)

../gobpack: gobpack.c
	$(CC) $(CFLAGS) -o ../gobpack $(LIBS) gobpack.c

../jnbpack: jnbpack.c
	$(CC) $(CFLAGS) -o ../jnbpack $(LIBS) jnbpack.c

../jnbunpack: jnbunpack.c
	$(CC) $(CFLAGS) -o ../jnbunpack $(LIBS) jnbunpack.c

clean:
	rm -f $(TARGETS) $(OBJS)
