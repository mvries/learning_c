/* This file contains functions to generate and manipulate int arrays. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "array_utils.h"

/* This function generates an array of random integers. */
int* make_random_array(int array_length)
{
    int* array = (int*) malloc(array_length * sizeof(int));
    srand(time(NULL));

    for(int i = 0; i < array_length; i++)
    {
        int random_number = rand();
        array[i] = random_number;
    }

    return array;
}

/* This function prints an array to the screen */
void print_array(int* array, int array_length)
{
    for(int i = 0; i < array_length; i++)
    {
	printf("%i", array[i]);
    }
}
