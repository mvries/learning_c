/* This script is a c implementation of the bubblesort algorithm. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>


/* Function to generate a randomly scrambled array of continues numbers. */
int* make_sorting_array(int array_size)
{
  // Make the array to sort and put on the heap:
  int* sorting_array = malloc(array_size*sizeof(int));

  // Populate array with integers:
  for (int i = 1; i <= array_size; i++)
    {
      sorting_array[i - 1] = i; 
    }

  // Then we scramble the array:
  time_t t;
  srand((unsigned) time(&t));
  for (int i = 0; i < array_size; i++)
    {
      int temp = sorting_array[i];
      int random_index = rand() % array_size;

      sorting_array[i] = sorting_array[random_index];
      sorting_array[random_index] = temp;
    }
  
  return sorting_array;
}


/* Function to prepare a screen for the program. */
int prepare_screen(SDL_Window** screen, SDL_Renderer** renderer, int window_height, int window_width)
{
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
    return 1;
  }

  *screen = SDL_CreateWindow("Bubblesort", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_height, window_width,0);
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
  int scaled_height;
  int scaled_width;
  int scaled_x;
  int scaled_y;
  int padding = 20;

  scaled_height = (window_height - padding * 2) / array_size;
  scaled_width = (window_width - padding * 2) / array_size;

  SDL_Rect new_point;
  for (int i = 0; i < array_size; i++)
    {
      int old_x = i;
      int old_y = sorting_array[i];

      scaled_x = padding + (i * scaled_width);
      scaled_y = ((window_height - (padding + scaled_height)) - (old_y * scaled_height)) + scaled_height;
      
      new_point.x = scaled_x;
      new_point.y = scaled_y;
      new_point.w = scaled_height;
      new_point.h = scaled_width;
      SDL_rect_array[i] = new_point;
    }
  return 0;
}


/* Function to draw the points in the SDL point array to the screen: */
int draw_SDL_array(SDL_Rect* SDL_array, int array_size, SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
  SDL_RenderDrawRects(renderer, SDL_array, array_size);
  SDL_RenderPresent(renderer);  
}


/* Main function of the script */
int main()
{
  // Size of the array to be sorted:
  int array_size = 10;

  // Generate array to be sorted:
  int* sorting_array = make_sorting_array(array_size);
  
  // Prepare the screen:
  int window_width = 820;
  int window_height = 820;
  SDL_Window* screen;
  SDL_Renderer* renderer;
  
  prepare_screen(&screen, &renderer, window_height, window_width);

  // Generate an array in heap to store the SDL rects:
  struct SDL_Rect
  {
    int x, y;
    int w, h;
  };

  SDL_Rect* SDL_rect_array = (SDL_Rect*)malloc(array_size * sizeof(SDL_Rect));

  // Update the array with the initial values:
  update_SDL_array(SDL_rect_array, sorting_array, array_size, window_width, window_height);

  // Draw the initial points to the screen:
  draw_SDL_array(SDL_rect_array, array_size, renderer);

  SDL_Delay(8000);
  return 0;
}

