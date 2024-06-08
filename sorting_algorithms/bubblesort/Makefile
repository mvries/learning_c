CC = gcc
CFLAGS = -I/usr/include/SDL2
DEPS = sorting_array.h SDL_rendering.h
LDLIBS = -lSDL2main -lSDL2

bubblesort.exe: bubblesort.c sorting_array.c SDL_rendering.c
	$(CC) -o bubblesort.exe bubblesort.c sorting_array.c SDL_rendering.c $(LDLIBS)

