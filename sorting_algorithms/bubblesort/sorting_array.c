/* This file contains the functions that are related to the generation and manipulation of an int array. */
#include "sorting_array.h"

// Function to generate a randomly scrambled array of contiguous integers.
int make_sorting_array(int* array, int array_size)
{

  // Populate array with integers:
  for (int i = 1; i <= array_size; i++)
    {
      array[i - 1] = i; 
    }

  // Then we scramble the array:
  time_t t;
  srand((unsigned) time(&t));
  for (int i = 0; i < array_size; i++)
    {
      int temp = array[i];
      int random_index = rand() % array_size;

      array[i] = array[random_index];
      array[random_index] = temp;
    }
  
  return 0;
}


/* Function that performs a bubblesort sweep of an input array */
int bubblesort_pass(int* sorting_array, int endpoint)
{
  for (int i = 1; i <= endpoint; i++)
  {
    int tmp;
    int previous = sorting_array[i - 1];
    int current = sorting_array[i];

    if (previous > current)
    {
      tmp = current;
      sorting_array[i] = previous;
      sorting_array[i - 1] = tmp;
    }
  }
  return 0;
}

