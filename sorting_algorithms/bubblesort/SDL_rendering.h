#ifndef SDL_RENDERING_H
#define SDL_RENDERING_H
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int prepare_screen(SDL_Window** screen, SDL_Renderer** renderer, int window_height, int window_width, char* program_name);
int update_SDL_array(SDL_Rect* SDL_rect_array, int* sorting_array, int array_size, int window_width, int window_height);
int draw_SDL_array(SDL_Rect* SDL_array, int array_size, SDL_Renderer* renderer);

#endif
