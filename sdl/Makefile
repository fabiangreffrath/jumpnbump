SDL_CFLAGS = `sdl-config --cflags`
CFLAGS = -Wall -ansi -pedantic -O2 -ffast-math -funroll-loops \
	-Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DNDEBUG \
	-I. -I.. $(SDL_CFLAGS) -DUSE_SDL -DUSE_NET

OBJS = gfx.o interrpt.o sound.o input.o
TARGET = ../sdl.a

all: $(TARGET)

$(TARGET): $(OBJS)
	ar cru $(TARGET) $(OBJS)
	ranlib $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
