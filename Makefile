OBJS = sdl/gfx.o sdl/interrpt.o sdl/sound.o sdl/input.o fireworks.o main.o menu.o
TARGET = jumpnbump


CC = gcc -g3
CFLAGS = -Wall -O2 -ffast-math -funroll-loops -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DUSE_SDL -I.
LIBS =  -lm -lSDL -lSDL_mixer

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(LIBS) -o $(TARGET) $(OBJS)


clean:
	rm -f $(TARGET) *.o *~ log

