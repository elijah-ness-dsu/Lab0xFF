#include "makegraphs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

int getBinaryValue(int *availablePoints, int availablePointCount)
{
	int value = 0;
    for(int i=0; i < availablePointCount; ++i)
    {
    	value += (int)pow(2,availablePoints[i] - 1);
	}
	return value;
}

int dynamicProg(double **matrix, int start, int end, int **availablePoints, int **subFirstPathValues, double **subCosts, int availablePointCount, int N)
{	
	if ((*subFirstPathValues)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start] != 0)
	{
		printf("sub exists at index %d: %d\n", getBinaryValue(*availablePoints, availablePointCount)*(N) + start, (*subFirstPathValues)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start]);
		return (*subFirstPathValues)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start]; 
	}
	else
	{
		if(availablePointCount == 1)
		{
			printf("Only one point left... start is %d\n", start);
			(*subFirstPathValues)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start] = (*availablePoints)[0];
			printf("subFirstPathValues = %d\n", (*subFirstPathValues)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start]);
			(*subCosts)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start] = (*matrix)[start*(N) + (*availablePoints)[0]] + (*matrix)[(*availablePoints)[0]*(N) + end];
			printf("Set subCost at index %d to value %f\n", getBinaryValue(*availablePoints, availablePointCount)*(N) + start, (*subCosts)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start]);
			
			return (*availablePoints)[0];
		}
		else
		{
			double bestCost = DBL_MAX; // Max value of an double
			int bestFirstPathVal;
			
			for(int i = 0; i < availablePointCount; ++i)
			{
				printf("i = %d\n", i);
				int tempFirstPathVal = 0;
				int tempRemovedPoint = (*availablePoints)[i];
				if((*availablePoints)[i] != (*availablePoints)[availablePointCount - 1])
				{
					(*availablePoints)[i] = (*availablePoints)[availablePointCount - 1];
				}
				else
				{
					(*availablePoints)[i] = 0;	
				}
				availablePointCount--;
				(*availablePoints)[availablePointCount] = 0;
				
				tempFirstPathVal = dynamicProg(matrix, tempRemovedPoint, end, availablePoints, subFirstPathValues, subCosts, availablePointCount, N);
				printf("tempFirstPathVal: %d\n", tempFirstPathVal);
				double tempCost = (*subCosts)[getBinaryValue(*availablePoints, availablePointCount)*(N) + tempRemovedPoint]; 
				printf("Binary tempCost pos: %d\n", getBinaryValue(*availablePoints, availablePointCount)*(N) + tempRemovedPoint);
				if(availablePointCount == N-1)
				{
					tempCost += (*matrix)[tempFirstPathVal];
					printf("addition of first node: %f\n", (*matrix)[tempFirstPathVal]);
				}
				if(tempCost < bestCost)
				{
					bestCost = tempCost;
					bestFirstPathVal = tempFirstPathVal;
					printf("New best cost: %f\n", bestCost);
				}
				
				availablePointCount++;
				(*availablePoints)[availablePointCount - 1] = (*availablePoints)[i];
				(*availablePoints)[i] = tempRemovedPoint;
				
			}
			
			
			(*subFirstPathValues)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start] = bestFirstPathVal;
			printf("Best path first = %d\n", bestFirstPathVal);
			(*subCosts)[getBinaryValue(*availablePoints, availablePointCount)*(N) + start] = bestCost;
			printf("Set subCost at index %d to the best value of %f\n", getBinaryValue(*availablePoints, availablePointCount)*(N) + start, bestCost);
			
			return bestFirstPathVal;
		}
	}
}

int main()
{
	double *matrix;
	int N = 5;
	
	generateRandomEuclideanCostMatrix(N, &matrix, 100, 18);
	
	int *availablePoints;
	int availablePointCount = N-1;
	availablePoints = calloc((N-1), sizeof(int));
	
	int *subFirstPathValues;
	subFirstPathValues = calloc(N * N * N, sizeof(int));
	
	double *subCosts;
	subCosts = malloc(N * N * N * sizeof(double));
	
	for (int i = 0; i < N - 1; ++i)
		availablePoints[i] = i+1;
	
	printf("Matrix of edges: \n");
	for(int i = 0; i < N*N; ++i)
		printf("%f ", matrix[i]);
	printf("\n--------------------\n\n");
	
	int result = dynamicProg(&matrix, 0, 0, &availablePoints, &subFirstPathValues, &subCosts, availablePointCount, N);
	
	printf("Best order: 0, %d, ", result); 
	
	for (int i = 0; i < N-2; ++i)
	{
		for(int j =0; j < availablePointCount; ++j)
		{
			if(availablePoints[j] == result)
			{
				availablePoints[j] = availablePoints[availablePointCount-1-i];
			}
		}
		result = subFirstPathValues[getBinaryValue(availablePoints, availablePointCount-i-1)*(N) + result];
		printf("%d, ", result);
	}
	
	printf("0\n");
	
	free(matrix);
	free(availablePoints);
	free(subFirstPathValues);
	free(subCosts);
	return 0;
}