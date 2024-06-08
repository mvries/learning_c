/* This file contains functions related to rendering with the SDL library */
#include "SDL_rendering.h"

/* Function to prepare a screen for the program. */
int prepare_screen(SDL_Window** screen, SDL_Renderer** renderer, int window_height, int window_width, char* program_name)
{
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
    return 1;
  }

  *screen = SDL_CreateWindow(program_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_height, window_width,0);
  if(!*screen)
    {
      fprintf(stderr, "Could not create window\n");
      return 1;
    }

  *renderer = SDL_CreateRenderer(*screen, -1, SDL_RENDERER_SOFTWARE);
  if(!*renderer)
    {
      fprintf(stderr, "Could not create renderer\n");
      return 1;
    }
  
  SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
  SDL_RenderClear(*renderer);
  SDL_RenderPresent(*renderer);
  return 0;
}


/* Function to update the SDL point array based on the sorting array. */
int update_SDL_array(SDL_Rect* SDL_rect_array, int* sorting_array, int array_size, int window_width, int window_height)
{
  // The rectangles are scaled to the screen so that they are represented properly.
  int scaled_height = 0;
  int scaled_width = 0;
  int scaled_x = 0;
  int scaled_y = 0;
  int padding = 50;
  SDL_Rect* new_point = NULL;

  scaled_height = (window_height - (padding * 2)) / array_size;
  scaled_width = (window_width - (padding * 2)) / array_size;

  for (int i = 0; i < array_size; i++)
    {
      int old_y = sorting_array[i];

      scaled_x = padding + (i * scaled_width);
      scaled_y = (window_height - padding) - (old_y * scaled_height);

      new_point = &SDL_rect_array[i];
      new_point->x = scaled_x;
      new_point->y = scaled_y;
      new_point->w = scaled_height;
      new_point->h = scaled_width;
    }
  return 0;
}


/* Function to draw the points in the SDL point array to the screen: */
int draw_SDL_array(SDL_Rect* SDL_array, int array_size, SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
  SDL_RenderDrawRects(renderer, SDL_array, array_size);
  SDL_RenderPresent(renderer);
  return 0;
}

