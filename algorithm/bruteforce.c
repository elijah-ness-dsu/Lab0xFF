// Include standard libraries
#include <string.h>
#include <float.h>

// Include local dependencies
#include "../utility/makegraph.h"
#include "../utility/changearray.h"

int *bestOrder;
int *currentOrder;

double bestTotal = DBL_MAX; // Max value of an double

double bruteForce(double **matrix, int start, int end)
{	
	if (start == end)
	{
		double currentTotal = 0;
		for (int i = 0; i < end; ++i)
			currentTotal += (*matrix)[currentOrder[i]*(end+1) + currentOrder[i + 1]];
		currentTotal += (*matrix)[currentOrder[end]*(end+1) + currentOrder[end + 1]];
		
		if (currentTotal < bestTotal)
		{
			bestTotal = currentTotal;
			memcpy(bestOrder, currentOrder, sizeof(int) * (end + 1));
		}
	}
	else
	{
    	for (int i = start; i < end; i++)
    	{
        	swapInts((&currentOrder[start + 1]), (&currentOrder[i] + 1));
        	bruteForce(matrix, start + 1, end);
        	swapInts((&currentOrder[start + 1]), (&currentOrder[i] + 1));
    	}
	}
}