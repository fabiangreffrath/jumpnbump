SDL_CFLAGS = `sdl2-config --cflags`
SDL_LIBS = `sdl2-config --libs`
CFLAGS = -Wall -O2 -ffast-math -funroll-loops -Dstricmp=strcasecmp \
	-Dstrnicmp=strncasecmp -DUSE_SDL -DNDEBUG -I. $(SDL_CFLAGS) \
	-DUSE_NET -DZLIB_SUPPORT -DBZLIB_SUPPORT
LIBS = -lm $(SDL_LIBS) -lSDL2_mixer -lSDL2_net -lbz2 -lz
SDL_TARGET = sdl.a
MODIFY_TARGET = gobpack jnbpack jnbunpack
OBJS = main.o menu.o filter.o network.o
TARGET = jumpnbump
BINARIES = $(TARGET) jumpnbump.svgalib jumpnbump.fbcon $(MODIFY_TARGET) \
	jnbmenu.tcl
PREFIX ?= /usr/local

.PHONY: data

all: $(BINARIES)

$(SDL_TARGET): globals.h
	$(MAKE) -C sdl

$(MODIFY_TARGET): globals.h
	$(MAKE) -C modify

$(TARGET): $(OBJS) $(SDL_TARGET) data globals.h
	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(SDL_TARGET)

$(OBJS): globals.h

globals.h: globals.pre
	sed -e "s#%%PREFIX%%#$(PREFIX)#g" < globals.pre > globals.h

jnbmenu.tcl: jnbmenu.pre
	sed -e "s#%%PREFIX%%#$(PREFIX)#g" < jnbmenu.pre > jnbmenu.tcl

data: jnbpack
	$(MAKE) -C data

clean:
	for dir in data modify sdl; do $(MAKE) clean -C $$dir; done
	$(RM) $(TARGET) *.o globals.h jnbmenu.tcl

install:
	mkdir -p $(PREFIX)/bin/
	mkdir -p $(PREFIX)/share/jumpnbump/
	mkdir -p $(PREFIX)/share/man/man6/
	install -m 755 $(BINARIES) $(PREFIX)/bin/
	install -m 644 data/jumpbump.dat \
		$(PREFIX)/share/jumpnbump/jumpbump.dat
	install -m 644 jumpnbump.6 $(PREFIX)/share/man/man6/

uninstall:
	for bin in $(BINARIES); do $(RM) $(PREFIX)/bin/$$bin; done
	$(RM) -r $(PREFIX)/share/jumpnbump
	$(RM) $(PREFIX)/share/man/man6/jumpnbump.6

doc:
	rman jumpnbump.6 -f HTML >jumpnbump.html
