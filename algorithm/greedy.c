// Derived from: https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/

// Include standard libraries
#include <stdlib.h>
#include <string.h>
#include <float.h>

// Include local dependencies
#include "../utility/changearray.h"

// Avoid redundant prototypes
#include "greedy.h"


void greedy(double **matrix, int N, int **bestOrder, double *bestTotal)
{	
	int *availablePoints = malloc((N-1) * sizeof(int));
	int availablePointCount = N-1;
	
	for (int i = 0; i < N - 1; ++i)
		availablePoints[i] = i+1;
	
	for(int i = 0; i < N - 1; ++i) 
	{
		double currentBestValue = DBL_MAX;
		int usedPoint;
		
		for(int j = 0; j < availablePointCount; ++j) 
		{
			double edgeValue = (*matrix)[(*bestOrder)[i]*(N) + availablePoints[j]];
			if(edgeValue < currentBestValue)
			{
				currentBestValue = edgeValue;
				(*bestOrder)[i+1] = availablePoints[j];
				usedPoint = j;
				
			}
		}
		
		*bestTotal = (*bestTotal) + currentBestValue;
		
		availablePoints[usedPoint] = availablePoints[availablePointCount-1];
		availablePointCount--;
	}
	
	*bestTotal = (*bestTotal) + (*matrix)[(*bestOrder)[N-1]*(N) + 0];
}

void greedyDummy(double **matrix, int N, int **bestOrder, double *bestTotal)
{
	
}