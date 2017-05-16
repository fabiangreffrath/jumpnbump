INCLUDES += -I..
CFLAGS += $(DEFINES) $(INCLUDES)
# Override libs, SDL2 not needed
LIBS =

OBJS = gobpack.o jnbpack.o jnbunpack.o
GOBPACK = ../gobpack$(EXE)
JNBPACK = ../jnbpack$(EXE)
JNBUNPACK = ../jnbunpack$(EXE)
TARGETS = $(GOBPACK) $(JNBPACK) $(JNBUNPACK)

all: $(TARGETS)

$(GOBPACK): gobpack.o
	$(CC) $(CFLAGS) -o $(GOBPACK) gobpack.o $(LDFLAGS) $(LIBS)

$(JNBPACK): jnbpack.o
	$(CC) $(CFLAGS) -o $(JNBPACK) jnbpack.o $(LDFLAGS) $(LIBS)

$(JNBUNPACK): jnbunpack.o
	$(CC) $(CFLAGS) -o $(JNBUNPACK) jnbunpack.o $(LDFLAGS) $(LIBS)

clean:
	$(RM) $(TARGETS) $(OBJS)
