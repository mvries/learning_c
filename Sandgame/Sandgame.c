//This C file contains the code for a falling Sand game that i made to learn C.

//Include statements for the various libraries that were used:
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

///////////////////////////////////////////////////////// START OF SDL FUNCTIONS ///////////////////////////////////////////////////////////////////////

// Here the functions that are responsible for setting up and interacting with the SDL library are shown:

//The following function initiates the SDL library and the events module that is needed:
void Init_SDL()
{
  if((SDL_Init(SDL_INIT_VIDEO)==-1))
    {
      printf("Could not initialize SDL: %s.\n", SDL_GetError());
      exit(-1);
    }
}

//This function creates a window to render the game on:
void Create_SDL_window(SDL_Window** window, char* program_title, int width, int heigth)
{
  *window = SDL_CreateWindow(program_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, heigth, SDL_WINDOW_OPENGL);

  if (window == NULL)
    {
      printf("Error, could not create the SDL_window");
      exit(-1);
    }
}

//This function sets up the SDL renderer:
void Create_SDL_renderer(SDL_Renderer** renderer, SDL_Window** window)
{
  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL)
    {
      printf("Error, could not create the SDL_renderer");
      exit(-1);
    }
}

//This function is used to clear the SDL window (To white colour).
void Clear_SDL_window(SDL_Renderer** renderer)
{
  SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
  SDL_RenderClear(*renderer);
  SDL_RenderPresent(*renderer);
}

//This function handels the keyboard input given to SDL:
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

//This function is responsible for handling the user mouse input:
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

//This function draws the sand particles (SDL_Rects) to the screen:
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
