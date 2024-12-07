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

//Global variables:
//Struct that stores SDL window + renderer related variables:
typedef struct
{
  SDL_Window* window;
  int window_width;
  int window_height;
  const char* program_title;
  SDL_Renderer* renderer;
} SDL_State;

///////////////////////////////////////////////////////// START OF SDL FUNCTIONS ///////////////////////////////////////////////////////////////////////
// Here the functions that are responsible for setting up and interacting with the SDL library are shown:

void Init_SDL()
{
  if((SDL_Init(SDL_INIT_VIDEO)==-1))
    {
      printf("Could not initialize SDL: %s.\n", SDL_GetError());
      exit(-1);
    }
}

void Create_SDL_window(SDL_State* state)
{
  state->window = SDL_CreateWindow(state->program_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, state->window_width, state->window_height, SDL_WINDOW_OPENGL);

  if (state->window == NULL)
    {
      printf("Error, could not create the SDL_window");
      exit(-1);
    }
}

void Create_SDL_renderer(SDL_State* state)
{
  state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);

  if (state->renderer == NULL)
    {
      printf("Error, could not create the SDL_renderer");
      exit(-1);
    }
}

void Clear_SDL_window(SDL_State* state)
{
  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
  SDL_RenderClear(state->renderer);
  SDL_RenderPresent(state->renderer);
}

//Master function to setup SDL and create window + renderer.
void SDL_Setup(SDL_State* state)
{
  Init_SDL();
  Create_SDL_window(state);
  Create_SDL_renderer(state);
  Clear_SDL_window(state);
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
void Handle_SDL_key_input(SDL_Event* event, int* run_code, SDL_Rect** particle_array, int* sand_particles, int** max_heigth_array, int* max_sand_particles, int* particle_size, SDL_State* state)
{
  switch (event->key.keysym.sym)
    {
    case SDLK_q:
      *run_code = 0;
      break;
    case SDLK_r:
      *sand_particles = 0;
      memset(*particle_array, 0, *max_sand_particles * sizeof(SDL_Rect));

       for (int i = 0; i < state->window_width; i++)
	 {
	   (*max_heigth_array)[i] = state->window_width - *particle_size;
	 }
      Clear_SDL_window(state);
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
void Draw_sand_to_screen(SDL_Rect** particle_array, int* sand_particles, SDL_State* state)
{
  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255); //White
  SDL_RenderClear(state->renderer);
  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255); //Black
  SDL_RenderDrawRects(state->renderer, *particle_array, *sand_particles);
  SDL_RenderPresent(state->renderer);
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

int main()
{
  //Initiate SDL_State struct for the Sandgame:
  SDL_State Sandgame_SDL_State;
  Sandgame_SDL_State.window = NULL;
  Sandgame_SDL_State.window_width = 400;
  Sandgame_SDL_State.window_height = 400;
  Sandgame_SDL_State.program_title = "Sandgame";
  Sandgame_SDL_State.renderer = NULL;
  
  //Master call function that handles the initiating of SDL window and renderer:
  SDL_Setup(&Sandgame_SDL_State);

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
  int* max_heigth_array = (int*) malloc(Sandgame_SDL_State.window_width * sizeof(int));

  //We initiate the height array so that particles drop to the bottom at first:
  for (int i = 0; i < Sandgame_SDL_State.window_width; i++)
    {
      max_heigth_array[i] = Sandgame_SDL_State.window_height - particle_size;
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
	      Handle_SDL_key_input(&event, &run_code, &particle_array, &sand_particles, &max_heigth_array, &max_sand_particles, &particle_size, &Sandgame_SDL_State);
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
      Draw_sand_to_screen(&particle_array, &sand_particles, &Sandgame_SDL_State);
      SDL_Delay(10);
    }
  free(max_heigth_array);
  free(particle_array);
  SDL_Quit();
  exit(0);
}
