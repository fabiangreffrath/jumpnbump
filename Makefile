OBJS = sdl/gfx.o sdl/interrpt.o sdl/sound.o sdl/input.o fireworks.o main.o menu.o
TARGET = jumpnbump

CC = gcc
CFLAGS = -Wall -O2 -ffast-math -funroll-loops -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DUSE_SDL -DNDEBUG -I. $(shell sdl-config --cflags)
LIBS = -lm $(shell sdl-config --libs) -lSDL_mixer

.PHONY: data

all: $(TARGET) pack unpack data

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

pack: modify/pack.o
	$(CC) $(LFLAGS) -o pack modify/pack.o $(LIBS)

unpack: modify/unpack.o
	$(CC) $(LFLAGS) -o unpack modify/unpack.o $(LIBS)

data: pack
	$(MAKE) -C data

clean:
	rm -f $(TARGET) pack unpack *.o sdl/*.o *~ log modify/*.o
	$(MAKE) -C data clean

install:
	install -o root -g games -m 755 $(TARGET) $(DESTDIR)/usr/games/$(TARGET)
	install -o root -g games -m 644 data/jumpbump.dat $(DESTDIR)/usr/share/jumpnbump/jumpbump.dat
