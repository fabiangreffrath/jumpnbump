OBJS = sdl/gfx.o sdl/interrpt.o sdl/sound.o sdl/input.o fireworks.o main.o menu.o
TARGET = jumpnbump


CC = gcc
CFLAGS = -Wall -O2 -ffast-math -funroll-loops -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DUSE_SDL -DNDEBUG -I. $(shell sdl-config --cflags)
LIBS = -lm $(shell sdl-config --libs) -lSDL_mixer

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) *.o sdl/*.o *~ log

