INCLUDES += -I..
CFLAGS += $(DEFINES) $(SDL_CFLAGS) $(INCLUDES)

OBJS = gfx.o interrpt.o sound.o input.o
TARGET = ../sdl.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cru $(TARGET) $(OBJS)
	ranlib $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)
