CC=gcc
CFLAGS=-Wall -Wextra -O2 $(shell sdl2-config --cflags)
LDFLAGS=$(shell sdl2-config --libs)

SOURCES=Sandgame.c

EXECUTABLE=Sandgame.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean 
