/* This is the main function of the bubble sort algortihm */
#include "bubblesort_utils.h"
#include <time.h>
#include <stdio.h>

int main()
{
    // Generate an array
    int array_length = 100;
    int* array = make_random_array(array_length);
    int search_index = array_length - 1;

    //Keep track of time
    clock_t start, end;
    double cpu_time_used;

    start  = clock();
    
    while (search_index > 1)
    {
        //perform bubblesort on the array
        for (int i = 0; i < search_index; i++)
        {
	    if (compare_items(array, i, i + 1 ) == 1)
	    {
		swap_items(array, i, i + 1);
	    }
        }
	search_index -= 1;
    }

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    // Print out the sorted array and runtime
    print_array(array, array_length);
    printf("Runtime: %f seconds\n", cpu_time_used);
    
    return(0);
}
