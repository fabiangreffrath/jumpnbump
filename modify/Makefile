INCLUDES += -I..
CFLAGS += $(DEFINES) $(INCLUDES)
# Override libs, SDL2 not needed
LIBS = -lm

OBJS = gobpack.o jnbpack.o jnbunpack.o
GOBPACK = ../gobpack$(EXE)
JNBPACK = ../jnbpack$(EXE)
JNBUNPACK = ../jnbunpack$(EXE)
TARGETS = $(GOBPACK) $(JNBPACK) $(JNBUNPACK)

all: $(TARGETS)

$(GOBPACK): gobpack.c
	$(CC) $(CFLAGS) -o $(GOBPACK) $(LDFLAGS) $(LIBS) gobpack.c

$(JNBPACK): jnbpack.c
	$(CC) $(CFLAGS) -o $(JNBPACK) $(LDFLAGS) $(LIBS) jnbpack.c

$(JNBUNPACK): jnbunpack.c
	$(CC) $(CFLAGS) -o $(JNBUNPACK) $(LDFLAGS) $(LIBS) jnbunpack.c

clean:
	$(RM) $(TARGETS) $(OBJS)
