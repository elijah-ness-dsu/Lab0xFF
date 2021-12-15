// USER PARAMETERS
int M = 10; // Number of ants
double pFactor = 1;
double decayFactor = 0.9;
int maxUnchangedTimeSteps = 10;
int seed = 239;
//

// Include Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// Avoid having to declare prototypes again
#include "antcolony.h"

double attraction(int k, int h, double **matrix, double **pheroMatrix, int N)
{
	return (1 + (*pheroMatrix)[k*N + h]) / (*matrix)[k*N + h];
}

void antColony(double **matrix, int N, int **bestOrder, int **currentOrder, double *bestCost)
{	
	double *pheroMatrix;
	double *newPheroMatrix;
	int *visited;
	
	visited = calloc(N, sizeof(int));
	
	pheroMatrix = calloc((N * N), sizeof(double));
	newPheroMatrix = calloc((N * N), sizeof(double));
	
	int unchangedTimeSteps = 1;
	
	srand(seed);
	
	while(unchangedTimeSteps <= maxUnchangedTimeSteps)
	{
		for(int i = 0; i < M; ++i)
		{
			double pathCost = 0;
			(*currentOrder)[0] = 0;
			visited[0] = 1;
			
			int h;
			
			for(int step = 1; step < N; ++step)
			{
				int k = (*currentOrder)[step-1];
				double totalA = 0;
				
				for(h = 0; h < N; ++h)
				{
					if(visited[h] == 0 && h != k)
					{
						totalA += attraction(k, h, matrix, &pheroMatrix, N);
					}
				}
				
				double Q = ((double)rand()/(double)(RAND_MAX));
				double cumulativeProb = 0;
				
				for(h = 0; h < N; ++h)
				{
					if(visited[h] == 0 && h != k)
					{
						cumulativeProb += attraction(k, h, matrix, &pheroMatrix, N)/totalA;
						if(Q < cumulativeProb)
							break;
					}
				}
				printf("Chose to go from node %d to node %d\n", k, h);
				(*currentOrder)[step] = h;
				visited[h] = 1;
				pathCost += (*matrix)[k*N + h];
			}
			
			pathCost += (*matrix)[h*N + 0];
			printf("Final cost of route: %f\n", pathCost);
			
			if(pathCost < *bestCost)
			{
				*bestCost = pathCost;
				memcpy((*bestOrder), (*currentOrder), sizeof(int) * (N+1));
				
				unchangedTimeSteps = 0;
				printf("New Best: %d %d %d %d %d was a cost of %f\n", (*bestOrder)[0], (*bestOrder)[1], (*bestOrder)[2], (*bestOrder)[3], (*bestOrder)[4], *bestCost);
			}
			
			for(int j = 0; j < N; ++j)
			{
				int k = (*currentOrder)[j];
				int h = (*currentOrder)[j+1];
				newPheroMatrix[k*N + h] += (1/pathCost) * pFactor;
			}
			
			visited = calloc(N, sizeof(int)); // Clear for next ant
		}
		
		for(int a = 0; a < N; ++a)
		{
			for(int b = 0; b < N; ++b)
			{
				pheroMatrix[a*N + b] *= decayFactor;
				pheroMatrix[a*N + b] += newPheroMatrix[a*N + b];
			}	
		}
		
		newPheroMatrix = calloc((N * N), sizeof(double)); // Clear for next time around
		
		printf("\n\n");
		unchangedTimeSteps += 1; // For testing above code
	}
	
	free(pheroMatrix);
	free(newPheroMatrix);
	free(visited);
}