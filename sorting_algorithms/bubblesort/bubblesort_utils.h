#ifndef BUBBLESORT_UTILS_H_
#define BUBBLESORT_UTILS_H_

// Function prototype for make random array
int* make_random_array(int array_length);

// Function to print the contents of the array
void print_array(int* array, int  array_length);

// Function to swap elements in an array
void swap_items(int* array, int position_1, int position_2);

// Function to compare elements in an array
int compare_items(int* array, int position_1, int position_2);
#endif /* BUBBLESORT_UTILS_H_ */
