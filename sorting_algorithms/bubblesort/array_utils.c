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
        int random_number = rand() % (1000 + 1 - 1) + 1;
        array[i] = random_number;
    }

    return array;
}

/* This function prints an array to the screen */
void print_array(int* array, int array_length)
{
    for(int i = 0; i < array_length; i++)
    {
	printf("%d\n", array[i]);
    }
}

/* This function swaps the position of two elements in an array */
void swap_items(int* array, int position_1, int position_2)
{
    int temp = array[position_1];
    array[position_1] = array[position_2];
    array[position_2] = temp;
}
