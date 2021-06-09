CC=gcc
CFLAGS=-std=c17 -pedantic -Wall
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
DEST=.
EXE=cube_shooter
INCLUDES= -I/usr/include/lua5.3 #`sdl2-config --cflags`
LIBS= -llua5.3 -lm # `sdl2-config --libs` -lSDL2_gfx

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(DEST)/$(EXE)

debug: CFLAGS += -g
debug: exe

profile_NOX: CFLAGS += -pg
profile_NOX: NOX

release: CFLAGS += -O2
release: exe

clean:
	@ - rm $(OBJECTS)
