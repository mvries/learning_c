/* This file contains a function to generate an array filled with random integers */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
