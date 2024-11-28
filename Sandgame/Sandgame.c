/*
This file contains the source code (C) for a falling sand game i made to learn c.
When running the game, click and drag the mouse anywhere on the screen to spawn a particle.

Author: Martijn de Vries
github: https://github.com/mvries
*/

//Include statements:
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

///////////////////////////////////////////////////////// START OF SDL FUNCTIONS ///////////////////////////////////////////////////////////////////////
// Here the functions that are responsible for setting up and interacting with the SDL library are shown:

/**
 * @brief Initializes the SDL2 library.
 *
 * This function initializes the SDL2 library for video. 
 * If initialization fails, it prints an error message and exits the program with a status of -1.
*/
void Init_SDL()
{
  if((SDL_Init(SDL_INIT_VIDEO)==-1))
    {
      printf("Could not initialize SDL: %s.\n", SDL_GetError());
      exit(-1);
    }
}

/**
 * @brief Creates an SDL window for the game.
 *
 * This function creates a new window with the specified title and dimensions.
 * 
 * @param window Pointer to the SDL_Window pointer where the created window will be stored.
 * @param program_title Title of the window.
 * @param width Width of the window.
 * @param height Height of the window.
 */
void Create_SDL_window(SDL_Window** window, char* program_title, int width, int heigth)
{
  *window = SDL_CreateWindow(program_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, heigth, SDL_WINDOW_OPENGL);

  if (window == NULL)
    {
      printf("Error, could not create the SDL_window");
      exit(-1);
    }
}

/**
 * @brief Creates an SDL renderer.
 *
 * This function creates a renderer associated with the given window.
 * 
 * @param renderer Pointer to the SDL_Renderer pointer where the created renderer will be stored.
 * @param window Pointer to the SDL_Window to render on.
 */
void Create_SDL_renderer(SDL_Renderer** renderer, SDL_Window** window)
{
  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL)
    {
      printf("Error, could not create the SDL_renderer");
      exit(-1);
    }
}

/**
 * @brief Clears the SDL window to a white color.
 *
 * This function sets the draw color to white and clears the current rendering target.
 * 
 * @param renderer Pointer to the SDL_Renderer.
 */
void Clear_SDL_window(SDL_Renderer** renderer)
{
  SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
  SDL_RenderClear(*renderer);
  SDL_RenderPresent(*renderer);
}

/**
 * @brief Handles keyboard input from the user.
 *
 * This function processes keyboard events:
 * - 'q' exits the game.
 * - 'r' restarts the simulation.
 * 
 * @param event Pointer to the SDL_Event containing the key event.
 * @param run_code Pointer to the variable controlling the game loop.
 * @param renderer Pointer to the SDL_Renderer.
 * @param particle_array Pointer to the array of particles.
 * @param sand_particles Pointer to the count of current sand particles.
 * @param max_heigth_array Pointer to the array of maximum heights for particles.
 * @param window_width Pointer to the width of the window.
 * @param max_sand_particles Pointer to the maximum number of sand particles.
 * @param particle_size Pointer to the size of each particle.
 */
void Handle_SDL_key_input(SDL_Event* event, int* run_code, SDL_Renderer** renderer, SDL_Rect** particle_array, int* sand_particles, int** max_heigth_array, int* window_width, int* max_sand_particles, int* particle_size)
{
  switch (event->key.keysym.sym)
    {
    case SDLK_q:
      *run_code = 0;
      break;
    case SDLK_r:
      *sand_particles = 0;
      memset(*particle_array, 0, *max_sand_particles * sizeof(SDL_Rect));

       for (int i = 0; i < *window_width; i++)
	 {
	   (*max_heigth_array)[i] = *window_width - *particle_size;
	 }
      Clear_SDL_window(renderer);
      break;
    }
}

/**
 * @brief Handles mouse input to place sand particles.
 *
 * This function updates the position of a sand particle based on the mouse's
 * current position when clicked or dragged. If the mouse coordinates are
 * even, it adjusts them to ensure proper placement.
 * 
 * @param mouse_x Pointer to the current X coordinate of the mouse.
 * @param mouse_y Pointer to the current Y coordinate of the mouse.
 * @param particle_array Pointer to the array of sand particles.
 * @param sand_particles Pointer to the count of current sand particles.
 * @param sand Pointer to the SDL_Rect representing the current sand particle.
 * @param particle_size Pointer to the size of each particle.
 */
void Handle_SDL_mouse_input(int* mouse_x, int* mouse_y, SDL_Rect** particle_array, int* sand_particles, SDL_Rect* sand, int* particle_size)
{  
  if ((*sand).y == *mouse_y)
    {
      (*sand).y = 0;
      return;
    }
  if ((*sand).x == *mouse_x)
    {
      (*sand).x = 0;
      return;
    }

  if (*mouse_x % 2 != 0)
    {
      *mouse_x -= 1;
    }

  (*sand).x = *mouse_x;
  (*sand).y = *mouse_y;
  (*sand).w = *particle_size;
  (*sand).h = *particle_size;
  
  (*particle_array)[*sand_particles] = *sand;
  *sand_particles += 1;
}

/**
 * @brief Renders the sand particles to the screen.
 *
 * This function draws all sand particles stored in the particle array
 * onto the SDL renderer and presents the updated frame.
 * 
 * @param particle_array Pointer to the array of sand particles.
 * @param sand_particles Pointer to the count of current sand particles.
 * @param renderer Pointer to the SDL_Renderer used for rendering.
 */
void Draw_sand_to_screen(SDL_Rect** particle_array, int* sand_particles, SDL_Renderer** renderer)
{
  SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255); //White
  SDL_RenderClear(*renderer);
  SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255); //Black
  SDL_RenderDrawRects(*renderer, *particle_array, *sand_particles);
  SDL_RenderPresent(*renderer);
}

///////////////////////////////////////////////////////// END OF SDL FUNCTIONS ///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////// START OF PHYSICS ENGINE CODE ////////////////////////////////////////////////////////////////

/**
 * @brief Handles the physics for sand particles.
 *
 * This function updates the vertical position of each sand particle based
 * on the maximum height allowed at its X coordinate. If a particle reaches
 * the maximum height, it will lower the maximum height to make space for
 * the next particle.
 * 
 * @param particle_array Pointer to the array of sand particles.
 * @param max_heigth_array Pointer to the array of maximum heights for particles.
 * @param sand_particles Pointer to the count of current sand particles.
 * @param particle_size Pointer to the size of each particle.
 */
void Handle_sand_physics(SDL_Rect** particle_array, int** max_heigth_array, int* sand_particles, int* particle_size)
{
  for (int i = 0; i < *sand_particles; i++)
    {
      if ((*particle_array)[i].y < (*max_heigth_array)[(*particle_array)[i].x])
	{
	  (*particle_array)[i].y += 1;
	}
      else if ((*particle_array)[i].y == (*max_heigth_array)[(*particle_array)[i].x])
	{
	  (*max_heigth_array)[(*particle_array)[i].x] -= *particle_size;
	}
    }
}

/////////////////////////////////////////////////////// END OF PHYSICS ENGINE CODE ////////////////////////////////////////////////////////////////////

/**
 * @brief Main function to run the falling sand game.
 *
 * This function initializes SDL, creates a window and renderer, and enters
 * the main loop to handle events and render the game state. It also manages
 * memory for particles and cleans up before exiting.
 * 
 * @return Exit status code.
 */
int main()
{
  //First the SDL library is initiated:
  Init_SDL();

  //Then we create an SDL window:
  SDL_Window* window = NULL;
  char* program_title = "Sandgame";
  int window_width = 400;
  int window_heigth = 400;
  Create_SDL_window(&window, program_title, window_width, window_heigth);

  //Then we create the renderer:
  SDL_Renderer* renderer = NULL;
  Create_SDL_renderer(&renderer, &window);

  //Wiping the screen before the start of the program:
  Clear_SDL_window(&renderer);

  //Variables needed during the execution of the code:
  //Sand related variables:
  int max_sand_particles = 10000;
  int sand_particles = 0;
  int particle_size = 2;

  //SDL related variables:
  int mouse_x, mouse_y;
  SDL_Event event;
  int dragging = 0;
  SDL_Rect sand;

  //Global arrays used by simulation:
  SDL_Rect* particle_array = (SDL_Rect*) malloc(max_sand_particles * sizeof(SDL_Rect));
  int* max_heigth_array = (int*) malloc(window_width * sizeof(int));

  //We initiate the height array so that particles drop to the bottom at first:
  for (int i = 0; i < window_width; i++)
    {
      max_heigth_array[i] = window_heigth - particle_size;
    }
  
  //Main loop of the sandgame:
  printf("Welcome to the Sandgame!\n");
  printf("press q to quit the program.\n");
  printf("Press r to restart the simulation.\n");
  int run_code = 1;
  while (run_code == 1)
    {
      //SDL event loop:
      while (SDL_PollEvent(&event))
	{
	  switch (event.type)
	    {
	    case SDL_KEYDOWN:
	      Handle_SDL_key_input(&event, &run_code, &renderer, &particle_array, &sand_particles, &max_heigth_array, &window_width, &max_sand_particles, &particle_size);
	      break;
	    case SDL_MOUSEBUTTONDOWN:
	      if (event.button.button == SDL_BUTTON_LEFT && sand_particles < max_sand_particles)
		{
		  SDL_GetMouseState(&mouse_x, &mouse_y);
		  Handle_SDL_mouse_input(&mouse_x, &mouse_y, &particle_array, &sand_particles, &sand, &particle_size);
		  dragging = 1;
		}
	      break;
	    case SDL_MOUSEBUTTONUP:
	      if (event.button.button == SDL_BUTTON_LEFT)
		{
		  dragging = 0;
		}
	      break;
	    }
	}
      if (dragging)
	{
	  SDL_GetMouseState(&mouse_x, &mouse_y);
	  if (sand_particles < max_sand_particles)
	    {
	      Handle_SDL_mouse_input(&mouse_x, &mouse_y, &particle_array, &sand_particles, &sand, &particle_size);
	    }
	}
      Handle_sand_physics(&particle_array, &max_heigth_array, &sand_particles, &particle_size);
      Draw_sand_to_screen(&particle_array, &sand_particles, &renderer);
      SDL_Delay(10);
    }
  free(max_heigth_array);
  free(particle_array);
  SDL_Quit();
  exit(0);
}
