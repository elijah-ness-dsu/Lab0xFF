#pragma once

// Function to swap to integer values in an array
// a: A pointer (memory location) to the first item to be swapped
// b: A pointer (memory location) to the other item to be swapped
void swapInts(int *a, int *b);

// Function to swap to double values in an array
// a: A pointer (memory location) to the first item to be swapped
// b: A pointer (memory location) to the other item to be swapped
void swapDoubles(double *a, double *b);

// Function to shuffle array values to create a seed-based random permutation
// array: The array to be shuffled
// n: The number of items in the array
// seed: The seed for RNG
void randomize(double array[], int n, int seed);