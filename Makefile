OBJS = sdl/gfx.o sdl/interrpt.o sdl/sound.o sdl/input.o fireworks.o main.o menu.o
TARGET = jumpnbump

CC = gcc
CFLAGS = -Wall -O2 -ffast-math -funroll-loops -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DUSE_SDL -DNDEBUG -I. $(shell sdl-config --cflags)
LIBS = -lm $(shell sdl-config --libs) -lSDL_mixer

.PHONY: data

all: $(TARGET) jnbpack jnbunpack gobpack data

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

jnbpack: modify/jnbpack.o
	$(CC) $(LFLAGS) -o jnbpack modify/jnbpack.o $(LIBS)

jnbunpack: modify/jnbunpack.o
	$(CC) $(LFLAGS) -o jnbunpack modify/jnbunpack.o $(LIBS)

gobpack: modify/gobpack.o
	$(CC) $(LFLAGS) -o gobpack modify/gobpack.o $(LIBS)

data: jnbpack
	$(MAKE) -C data

clean:
	rm -f $(TARGET) jnbpack jnbunpack *.o sdl/*.o *~ log modify/*.o
	$(MAKE) -C data clean

install:
	chmod +x jnbmenu.tcl
	mkdir -p $(DESTDIR)/usr/games/
	mkdir -p $(DESTDIR)/usr/share/jumpnbump/
	install -o root -g games -m 755 $(TARGET) jumpnbump.fbcon jumpnbump.svgalib jnbpack jnbunpack jnbmenu.tcl $(DESTDIR)/usr/games/
	install -o root -g games -m 644 data/jumpbump.dat $(DESTDIR)/usr/share/jumpnbump/jumpbump.dat

uninstall:
	rm $(DESTDIR)/usr/games/jnbpack $(DESTDIR)/usr/games/jnbunpack $(DESTDIR)/usr/games/jumpnbump $(DESTDIR)/usr/games/jnbmenu.tcl
