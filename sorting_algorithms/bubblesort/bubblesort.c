/* This script is a c implementation of the bubblesort algorithm with 2D rendering. */
#include <stdio.h>
#include <stdlib.h>

#include "sorting_array.h"
#include "SDL_rendering.h"


/* Main function of the script */
int main()
{
  // Generation of the sorting array:
  int array_size = 200;
  int sorting_array[array_size];
  make_sorting_array(sorting_array, array_size);
  
  // Setting up the screen for the program:
  int window_width = 900;
  int window_height = 900;
  SDL_Window* screen = NULL;
  SDL_Renderer* renderer = NULL;
  char program_name[] = "Bubblesort";
  prepare_screen(&screen, &renderer, window_height, window_width, program_name);

  // Generation of the SDL rect array:
  SDL_Rect SDL_rect_array[array_size];
  update_SDL_array(SDL_rect_array, sorting_array, array_size, window_width, window_height);

  // Draw the initial points to the screen:
  draw_SDL_array(SDL_rect_array, array_size, renderer);

  // Main bubblesort loop:
  int run_program = 1;
  while (run_program)
    {
      printf("Please choose one of the following: \n");
      printf("1. input 'q' to quit.\n");
      printf("2. input 's' to start.\n");
      char user_input;
      user_input = getchar();

      if (user_input == 'q')
      {
	run_program = 0;
	break;
      }
      else if (user_input == 's')
      {
	// main algorithm body is here.
	int endpoint = array_size - 1;

	for (int i = 0; i < array_size; i++ )
	{
	  bubblesort_pass(sorting_array, endpoint);
	  update_SDL_array(SDL_rect_array, sorting_array, array_size, window_width, window_height);
	  draw_SDL_array(SDL_rect_array, array_size, renderer);
	  endpoint -= 1;
	  SDL_Delay(100);
	}
	printf("Done!\n");
	SDL_Delay(2000);
	run_program = 0;
      }
      else
      {
	printf("Invalid input quiting... \n");
	run_program = 0;
      }
    }

  return 0;
}

