// Derived from: https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/

// Include standard libraries
#include <string.h>

// Include local dependencies
#include "../utility/changearray.h"

// Avoid redundant prototypes
#include "bruteforce.h"

void bruteForce(double **matrix, int start, int end, int **bestOrder, int **currentOrder, double *bestTotal)
{	
	if (start == end)
	{
		double currentTotal = 0;
		for (int i = 0; i < end; ++i)
			currentTotal += (*matrix)[(*currentOrder)[i]*(end+1) + (*currentOrder)[i + 1]];
		currentTotal += (*matrix)[(*currentOrder)[end]*(end+1) + (*currentOrder)[end + 1]];
		
		if (currentTotal < *bestTotal)
		{
			*bestTotal = currentTotal;
			memcpy((*bestOrder), (*currentOrder), sizeof(int) * (end + 1));
		}
	}
	else
	{
    	for (int i = start; i < end; i++)
    	{
        	swapInts((&(*currentOrder)[start + 1]), (&(*currentOrder)[i] + 1));
        	bruteForce(matrix, start + 1, end, bestOrder, currentOrder, bestTotal);
        	swapInts((&(*currentOrder)[start + 1]), (&(*currentOrder)[i] + 1));
    	}
	}
}