// Code derived from https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/

// Include standard libraries
#include <stdlib.h>

// Avoid having to repeat prototypes
#include "changearray.h"

// Function to swap to integer values in an array
// a: A pointer (memory location) to the first item to be swapped
// b: A pointer (memory location) to the other item to be swapped
void swapInts(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to swap to double values in an array
// a: A pointer (memory location) to the first item to be swapped
// b: A pointer (memory location) to the other item to be swapped
void swapDoubles(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

// Function to shuffle array values to create a seed-based random permutation
// array: The array to be shuffled
// n: The number of items in the array
// seed: The seed for RNG
void randomize(double array[], int n, int seed)
{
	// Seed the random generator
    srand(seed);
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first two element that's why i > 1
    for (int i = n - 1; i > 1; --i)
    {
        // Pick a random index from 1 to i
        int j = (rand() % i) + 1;
 
        // Swap array[i] with the element at random index
        swapDoubles(&array[i], &array[j]);
    }
}