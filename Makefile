SDL_CFLAGS = `sdl-config --cflags`
SDL_LIBS = `sdl-config --libs`
CFLAGS = -Wall -O2 -ffast-math -funroll-loops -Dstricmp=strcasecmp \
	-Dstrnicmp=strncasecmp -DUSE_SDL -DNDEBUG -I. $(SDL_CFLAGS) -DUSE_NET
LIBS = -lm $(SDL_LIBS) -lSDL_mixer -lSDL_net
SDL_TARGET = sdl.a
MODIFY_TARGET = gobpack jnbpack jnbunpack
OBJS = fireworks.o main.o menu.o filter.o
TARGET = jumpnbump
BINARIES = $(TARGET) jumpnbump.svgalib jumpnbump.fbcon $(MODIFY_TARGET) \
	jnbmenu.tcl
PREFIX ?= /usr/local

.PHONY: data

all: $(BINARIES)

$(SDL_TARGET): globals.h
	cd sdl && make

$(MODIFY_TARGET): globals.h
	cd modify && make

$(TARGET): $(OBJS) $(SDL_TARGET) data globals.h
	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(SDL_TARGET)

$(OBJS): globals.h

globals.h: globals.pre
	sed -e "s#%%PREFIX%%#$(PREFIX)#g" < globals.pre > globals.h

jnbmenu.tcl: jnbmenu.pre
	sed -e "s#%%PREFIX%%#$(PREFIX)#g" < jnbmenu.pre > jnbmenu.tcl

data: jnbpack
	cd data && make

clean:
	cd sdl && make clean
	cd modify && make clean
	cd data && make clean
	rm -f $(TARGET) *.o globals.h jnbmenu.tcl

install:
	mkdir -p $(PREFIX)/games/
	mkdir -p $(PREFIX)/share/jumpnbump/
	mkdir -p $(PREFIX)/share/man/man6/
	install -o root -g games -m 755 $(BINARIES) $(PREFIX)/games/
	install -o root -g games -m 644 data/jumpbump.dat \
	$(PREFIX)/share/jumpnbump/jumpbump.dat
	install -o root -g root -m 644 jumpnbump.6 $(PREFIX)/share/man/man6/

uninstall:
	cd $(PREFIX)/games && rm -f $(BINARIES)
	rm -rf $(PREFIX)/share/jumpnbump
	rm -f $(PREFIX)/share/man/man6/jumpnbump.6

doc:
	rman jumpnbump.6 -f HTML >jumpnbump.html
