CC=gcc
CFLAGS=-std=c17 -pedantic -Wall -fPIC
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
DEST=.
NAME=cubes
INCLUDES= -I/usr/include/lua5.3 `sdl2-config --cflags`
LIBS= -llua5.3 -lm `sdl2-config --libs` -lSDL2_gfx

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

exe: $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o $(DEST)/$(NAME)

lib: $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -shared -fPIC  -o $(DEST)/$(NAME).so

debug: CFLAGS += -g
debug: lib

profile_NOX: CFLAGS += -pg
profile_NOX: NOX

release: CFLAGS += -O2
release: exe

clean:
	@ - rm $(OBJECTS)
