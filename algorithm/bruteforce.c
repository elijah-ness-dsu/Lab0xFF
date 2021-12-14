// Derived from: https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/

// Include standard libraries
#include <string.h>

// Include local dependencies
#include "../utility/changearray.h"

// Avoid redundant prototypes
#include "bruteforce.h"

void bruteForce(double **matrix, int start, int N, int **bestOrder, int **currentOrder, double *bestTotal)
{	
	if (start == N-1)
	{
		double currentTotal = 0;
		for (int i = 0; i < N-1; ++i)
			currentTotal += (*matrix)[(*currentOrder)[i]*(N) + (*currentOrder)[i + 1]];
		currentTotal += (*matrix)[(*currentOrder)[N-1]*(N) + (*currentOrder)[N]];
		
		if (currentTotal < *bestTotal)
		{
			*bestTotal = currentTotal;
			memcpy((*bestOrder), (*currentOrder), sizeof(int) * (N));
		}
	}
	else
	{
    	for (int i = start; i < N-1; i++)
    	{
        	swapInts((&(*currentOrder)[start + 1]), (&(*currentOrder)[i] + 1));
        	bruteForce(matrix, start + 1, N, bestOrder, currentOrder, bestTotal);
        	swapInts((&(*currentOrder)[start + 1]), (&(*currentOrder)[i] + 1));
    	}
	}
}