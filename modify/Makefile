CFLAGS += -I..
# Override libs, SDL2 not needed
LIBS = -lm

OBJS = gobpack.o jnbpack.o jnbunpack.o
TARGETS = ../gobpack ../jnbpack ../jnbunpack

all: $(TARGETS)

../gobpack: gobpack.c
	$(CC) $(CFLAGS) -o ../gobpack $(LDFLAGS) $(LIBS) gobpack.c

../jnbpack: jnbpack.c
	$(CC) $(CFLAGS) -o ../jnbpack $(LDFLAGS) $(LIBS) jnbpack.c

../jnbunpack: jnbunpack.c
	$(CC) $(CFLAGS) -o ../jnbunpack $(LDFLAGS) $(LIBS) jnbunpack.c

clean:
	$(RM) $(TARGETS) $(OBJS)
